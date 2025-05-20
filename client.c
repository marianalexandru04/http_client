#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "buffer.h"
#include "parson.h"
#include <stdbool.h>

#define MAX_COOKIES 10
typedef struct {
    char cookies[20][100];
    int count;
}Cookies;

typedef struct {
    char *name;
    char *password;
} User;

void print_messages(char *msg1, char *msg2) {
    // if (msg1 != NULL)
    //     printf("%s\n", msg1);
    // if (msg2 != NULL)
    //     printf("%s\n", msg2);
}
void add_cookie(char **cookies, char *s_cookie, char *msgrecv) {
    char * ptr = strstr(msgrecv, "Set-Cookie:");
    if (ptr != NULL) {
        strcpy(s_cookie, ptr + 4);
        char *ptr2 = strchr(s_cookie,';');
        *ptr2 = '\0';
        strcpy(cookies[0], s_cookie);
    }
}

char *json_builder(char data[][500], char names[][50]) {
    JSON_Value *root_value = json_value_init_object();
    if (root_value == NULL) {
        fprintf(stderr, "Error: json_value_init_object failed in json_builder\n");
        return NULL;
    }
    JSON_Object *root_object = json_value_get_object(root_value);

    int i = 0;
    while (data[i][0] != '\0') {
        if (strstr(names[i], "num_movies???") != NULL) {
            // int cnt;
            // char *token = strtok(data[i], " ");
            // sscanf(token, "%d", &cnt);
            // for (size_t j = 0; j < cnt; j++) {
            //     int id;
            //     token = strtok(NULL, " ");
            //     sscanf(token, "%d", &id);
            //     json_set_number(movie, )
            // }
        } else if(strstr(names[i], "year") != NULL) {

            int year;
            sscanf(data[i], "%d", &year);
            json_object_set_number(root_object, names[i], year);
        } else if(strstr(names[i], "rating") != NULL) {
            double rating;
            sscanf(data[i], "%lf", &rating);
            // printf("%lf\n", rating);
            json_object_set_number(root_object, names[i], rating);
        } else {
            json_object_set_string(root_object, names[i], data[i]);
        }
        i++;
    }
    char *json_string = json_serialize_to_string(root_value);
    json_value_free(root_value);
    return json_string;
}


void zero_matrix(char types[][500], char names[][50]) {
    int i = 0;
    while (types[i][0] != '\0') {
        memset(types[i], 0, 50);
        i++;
    }
    int j = 0;
    while (names[j][0] != '\0') {
        memset(names[j], 0, 50);
        j++;
    }
}
char* replace_char(char* str, char find, int replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}
void add_user_and_password(User **users, char* username, char* password, int * usercount) {
    users[*usercount]->name = malloc(sizeof(char) * strlen(username));
    users[*usercount]->password = malloc(sizeof(char) * strlen(password));
    strcpy(users[*usercount]->name, username);
    strcpy(users[*usercount]->password, password);
    (*usercount) ++;

}
bool isUser(User **users, char* username, int * usercount) {
    for(size_t i = 0; i < (*usercount); i++) {
        if(strcmp(username, users[i]->name) == 0)
            return true;
    }
    return false;
}
void add_movie_to_collection(char *collection_id, char *movie_id,
                                char *host, char **cookies, int sockfd)
{
    char url[200];
    snprintf(url, sizeof(url), "/api/v1/tema/library/collections/%s/movies", collection_id);

    char json_string[100];
    int id2;
    sscanf(movie_id, "%d", &id2);
    sprintf(json_string, "{\"id\":%d}", id2);

    char *message = compute_post_request(host, url, "application/json",  json_string, cookies, MAX_COOKIES);
    send_to_server(sockfd, message);
    char *msgrecv = receive_from_server(sockfd);
    if (strstr(msgrecv, "already exists")) {
        printf("ERROR: movie already added\n");
    } else if (strstr(msgrecv, "400 BAD REQUEST")) {
        printf("ERROR: datele introduse sunt incomplete\n");
    } else {
        printf("SUCCESS: Film adăugat în colecție\n");
    }

    print_messages(message, msgrecv);

    //add_cookie(cookies, session_cookies, msgrecv); ???

    free(message);
    free(msgrecv);

}

bool space_check(char *var) {
    int i = 0;
    while(var[i] != '\0') {
        if(var[i] == ' ')
            return true;
        i++;
    }
    return false;
}
int main(int argc, char *argv[])
{
    int sockfd;

    // Cookies cookies;
    // cookies.count = 0;
    char **cookies = calloc(10, sizeof(char*));
    cookies[0] = calloc(200, sizeof(char));
    cookies[1] = calloc(200, sizeof(char));
    cookies[1][0] = '\0';
    cookies[0][0] = '\0';


    // int usercount = 0;
    // User **users;
    // users = malloc(sizeof(User*) * 50);
    

    setvbuf(stdout, NULL, _IONBF, 0);

    char * host_ip = "63.32.125.183";
    char * host = "63.32.125.183:8081";
    int portno = 8081;
    sockfd = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);

    char session_cookies[200];
    
    
    char matrix[6][500];
    matrix[0][0] = '\0';
    char matrix_names[6][50];
    matrix_names[0][0] = '\0';

    int ok = 0;
    

    char stdincmd[1000];
    while (1) {
        char *msgrecv = NULL;
        char *message = NULL;
        buffer buff = buffer_init();
        memset(stdincmd, 0, sizeof(stdincmd));
        fgets(stdincmd, sizeof(stdincmd), stdin);
        buffer_add(&buff, stdincmd, sizeof(stdincmd));

        if (strcmp(buff.data, "exit\n") == 0) {
            buffer_destroy(&buff);
            break;
        } else 
        // -------------------- login admin----------------------
        if (strcmp(buff.data, "login_admin\n") == 0) {
            printf("username=\n");
            char username[50];
            fgets(username, sizeof(username), stdin);
            replace_char(username, '\n', '\0');

            printf("password=\n");
            char password[50];
            fgets(password, sizeof(password), stdin);
            replace_char(password, '\n', '\0');

            zero_matrix(matrix, matrix_names);

            strcpy(matrix[0], username);
            strcpy(matrix[1], password);
            matrix[2][0] = '\0';
            strcpy(matrix_names[0], "username");
            strcpy(matrix_names[1], "password");
            matrix_names[2][0] = '\0';

            char *json_string = json_builder(matrix, matrix_names);
            char *url = "/api/v1/tema/admin/login";
            message = compute_post_request(host, url, "application/json", json_string, cookies, MAX_COOKIES);

            send_to_server(sockfd, message);

            msgrecv = receive_from_server(sockfd);

            print_messages(message, msgrecv);
            if (strstr(msgrecv, "Admin logged in successfully")) {
                printf("SUCCESS: Admin autentificat cu succes\n");
            } else {
                printf("ERROR: admin already logged in\n");
            }
            
            add_cookie(cookies, session_cookies, msgrecv);


        } else 
        // -------------------- add user----------------------
        if (strcmp(buff.data, "add_user\n") == 0) {
            printf("username=\n");
            char username[50];
            fgets(username, sizeof(username), stdin);
            replace_char(username, '\n', '\0');
            printf("password=\n");
            char password[50];
            fgets(password, sizeof(password), stdin);
            replace_char(password, '\n', '\0');

            zero_matrix(matrix, matrix_names);
            strcpy(matrix[0], username);
            strcpy(matrix[1], password);
            matrix[2][0] = '\0';
            strcpy(matrix_names[0], "username");
            strcpy(matrix_names[1], "password");
            matrix_names[2][0] = '\0';

            char *json_string = json_builder(matrix, matrix_names);

            if (json_string == NULL) {
                fprintf(stderr, "Error: Failed to create JSON payload for add_user.\\n");
            } else {
                char *url = "/api/v1/tema/admin/users";
                message = compute_post_request(host, url, "application/json", json_string, cookies, MAX_COOKIES);

                send_to_server(sockfd, message);

                json_free_serialized_string(json_string);
            }

            msgrecv = receive_from_server(sockfd);
            if (space_check(username) || space_check(password)) {
                printf("ERROR: Username or password should not contain spaces\n");
                continue;
            }
            if (strstr(msgrecv, "User already exists")) {
                printf("ERROR: user already exists\n");
            } else if (strstr(msgrecv, "Username or password should not contain spaces")) {
                printf("Username or password should not contain spaces\n");
            } else if (strstr(msgrecv, "error")) {
                printf("ERROR: unknown error\n");
            } else {
                printf("SUCCES: user added\n");
            }

            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);

        } else 
        // --------------------- get_users ---------------------
        if (strcmp(buff.data, "get_users\n") == 0) {

            char *url = "/api/v1/tema/admin/users";

            message = compute_get_request(host, url, NULL, cookies, MAX_COOKIES);

            send_to_server(sockfd, message);

            msgrecv = receive_from_server(sockfd);

            char *content_ptr = strstr(msgrecv, "{\"users");
            JSON_Value *root_value = json_parse_string(content_ptr);
            JSON_Object *root_object = json_value_get_object(root_value);
            JSON_Array *users = json_object_get_array(root_object, "users");

            size_t count = json_array_get_count(users);

            if (strstr(msgrecv, "200 OK")) {
                printf("SUCCESS: Lista utilizatorilor\n");
                for (size_t i = 0; i < count; i++) {
                    JSON_Object *user = json_array_get_object(users, i);
                    int id = (int)json_object_get_number(user, "id");
                    const char *username = json_object_get_string(user, "username");
                    const char *password = json_object_get_string(user, "password");

                    printf("#%d %s:%s\n", id, username, password);
                }
            }
            else 
                printf("ERROR: nu s-a putut accesa lista cu utilizatori\n");


            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);
            json_value_free(root_value);
        } else 
        // --------------------- delete_user ---------------------
        if (strcmp(buff.data, "delete_user\n") == 0) {
            /*
                char * host_ip = "63.32.125.183";
                int portno = 8081;
            */
            printf("username=\n");
            char username[50];
            fgets(username, sizeof(username), stdin);
            replace_char(username, '\n', '\0');

            char url[100];
            snprintf(url, sizeof(url), "/api/v1/tema/admin/users/%s", username);

            message = compute_delete_request(host, url, cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);

            if (strstr(msgrecv, "200 OK") != NULL) {
                printf("SUCCESS: Utilizator șters\n");
            } else {
                printf("ERROR: nu exista utilizatorul\n");
            }
            print_messages(message, msgrecv);

            add_cookie(cookies, session_cookies, msgrecv);

        } else
        // --------------------- logout_admin ---------------------
        if (strcmp(buff.data, "logout_admin\n") == 0) {
            /*
                GET /api/v1/tema/admin/logout
            */
            char *url = "/api/v1/tema/admin/logout";


            // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
            message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);

            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);

            if (strstr(msgrecv, "Admin logged out successfully") != NULL) {
                printf("SUCCES: Admin logged out successfully\n");
            } else {
                printf("ERROR: couldn't log out the admin\n");
            }


        } else
        // --------------------- login ---------------------
        if (strcmp(buff.data, "login\n") == 0) {
            /*
                POST /api/v1/tema/user/login
                Tip payload:

                application/json
                Payload:

                {
                "admin_username": String,
                "username": String,
                "password": String
                }

            */
            char *url = "/api/v1/tema/user/login";

            printf("admin_username=\n");
            char admin_username[50];
            fgets(admin_username, sizeof(admin_username), stdin);
            replace_char(admin_username, '\n', '\0');

            printf("username=\n");
            char username[50];
            fgets(username, sizeof(username), stdin);
            replace_char(username, '\n', '\0');

            printf("password=\n");
            char password[50];
            fgets(password, sizeof(password), stdin);
            replace_char(password, '\n', '\0');

            zero_matrix(matrix, matrix_names);

            strcpy(matrix[0], admin_username);
            strcpy(matrix[1], password);
            strcpy(matrix[2], username);
            matrix[3][0] = '\0';

            strcpy(matrix_names[0], "admin_username");
            strcpy(matrix_names[1], "password");
            strcpy(matrix_names[2], "username");
            matrix_names[3][0] = '\0';

            char *json_string = json_builder(matrix, matrix_names);
            if (json_string == NULL) {
                fprintf(stderr, "Error: Failed to create JSON payload for user login.\n");
            } else {
                message = compute_post_request(host, url, "application/json",  json_string, cookies, MAX_COOKIES);
                send_to_server(sockfd, message);
                json_free_serialized_string(json_string);
            }
            msgrecv = receive_from_server(sockfd);

            print_messages(message, msgrecv);

            add_cookie(cookies, session_cookies, msgrecv);



            if (strstr(msgrecv, "User logged in successfully") != NULL) {
                printf("SUCCES: User logged in successfully\n");
            } else {
                printf("ERROR: couldn't log in the user\n");
            }



        } else
        // --------------------- get_access  ---------------------
        if (strcmp(buff.data, "get_access\n") == 0) {
            /*
                GET /api/v1/tema/library/access
                SUCCESS: Token JWT primit

            */
            char *url = "/api/v1/tema/library/access";

            // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
            message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);


            char * ptr = strstr(msgrecv, "\"token\":\"");
            strcpy(session_cookies, ptr + 9);
            char *ptr2 = strchr(session_cookies,'"');
            *ptr2 = '\0';
            strcpy(cookies[1], session_cookies);

            if (ptr != NULL) {
                printf("SUCCESS: Token JWT primit\n");
            } else {
                printf("ERROR: token-ul jwt nu a fost primit\n");
            }
            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);


        } else
        // --------------------- get_movies  ---------------------
        if (strcmp(buff.data, "get_movies\n") == 0) {
            /*
                GET /api/v1/tema/library/movies
                SUCCESS: Token JWT primit

            */
            char *url = "/api/v1/tema/library/movies";

            // sockfd = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);

            
            // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
            message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);

            char *content_ptr = strstr(msgrecv, "{\"movies");
            JSON_Value *root_value = json_parse_string(content_ptr);
            JSON_Object *root_object = json_value_get_object(root_value);
            JSON_Array *movies = json_object_get_array(root_object, "movies");

            size_t count = json_array_get_count(movies);

            if (strstr(msgrecv, "\"movies\":[") != NULL) {
                printf("SUCCESS: Lista filmelor\n");
                for (size_t i = 0; i < count; i++) {
                    JSON_Object *movie = json_array_get_object(movies, i);
                    int id = (int)json_object_get_number(movie, "id");
                    const char *title = json_object_get_string(movie, "title");

                    printf("#%d %s\n", id, title);
                }
            } else {
                printf("ERROR: serverul nu a intors lista cu filme\n");
            }


            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);

            json_value_free(root_value);


        } else
        // --------------------- get_movie  ---------------------
        if (strcmp(buff.data, "get_movie\n") == 0) {
            /*
                GET /api/v1/tema/library/movies/:movieId
                {
                    "id": Number,
                    "title": String,
                    "year": Number,
                    "description": String,
                    "rating": Number
                }


                SUCCESS: Detalii film
                title: The Dark Knight
                year: 2010
                description: sci-fi
                rating: 8.8
            */
            char *url = "/api/v1/tema/library/movies/";
            printf("id=\n");
            char id[10];
            fgets(id, sizeof(id), stdin);
            replace_char(id, '\n', '\0');

            message = compute_get_request(host, url, id,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);



            if (strstr(msgrecv, "\"id\"") != NULL) {
                // {"description":"test","id":75707,"rating":"6.0","title":"test","user_id":27291,"year":2000}
                char *content_ptr = strstr(msgrecv, "{\"description");
                JSON_Value *root_value = json_parse_string(content_ptr);
                JSON_Object *root_object = json_value_get_object(root_value);


                const char *description = json_object_get_string(root_object, "description");
                const char *rating = json_object_get_string(root_object, "rating");
                const char *title = json_object_get_string(root_object, "title");
                int year = (int)json_object_get_number(root_object, "year");

                printf("SUCCESS: Detalii film\n"
                    "title: %s\n"
                    "year: %d\n"
                    "description: %s\n"
                    "rating: %s\n",
                    title, year, description, rating);

                json_value_free(root_value);

            } else if (strstr(msgrecv, "Movie not found") != NULL) {
                 printf("ERROR: Movie not found, invalid id\n");
            } else {
                printf("ERROR: serverul zice ca nu am acces la filme\n");
            }

            // print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);

        } else 
        // -------------------- add_movie ----------------------
        if (strcmp(buff.data, "add_movie\n") == 0) {
            char *url = "/api/v1/tema/library/movies";
            printf("title=\n");
            char title[50];
            fgets(title, sizeof(title), stdin);
            replace_char(title, '\n', '\0');

            printf("year=\n");
            char year[10];
            fgets(year, sizeof(year), stdin);
            replace_char(year, '\n', '\0');

            printf("description=\n");
            char description[500];
            fgets(description, sizeof(description), stdin);
            replace_char(description, '\n', '\0');

            printf("rating=\n");
            char rating[10];
            fgets(rating, sizeof(rating), stdin);
            replace_char(rating, '\n', '\0');

            double rat;
            sscanf(rating, "%lf", &rat);

            zero_matrix(matrix, matrix_names);

            strcpy(matrix[0], title);
            strcpy(matrix[1], year);
            strcpy(matrix[2], description);
            strcpy(matrix[3], rating);
            // matrix[3] = (void*)&rats;
            matrix[4][0] = '\0';

            strcpy(matrix_names[0], "title");
            strcpy(matrix_names[1], "year");
            strcpy(matrix_names[2], "description");
            strcpy(matrix_names[3], "rating");
            matrix_names[4][0] = '\0';

            char *json_string = json_builder(matrix, matrix_names);

            if (json_string == NULL) {
                fprintf(stderr, "Error: Failed to create JSON payload for add_movie.\n");
            } else {
                message = compute_post_request(host, url, "application/json",  json_string, cookies, MAX_COOKIES);
                send_to_server(sockfd, message);
                json_free_serialized_string(json_string);
            }

            msgrecv = receive_from_server(sockfd);
            if (rat > 10.0 || rat < 0.0) {
                printf("ERROR: rating invalid\n");
                continue;
            }
            if (strstr(msgrecv, "Movie already exists")) {
                printf("ERROR: movie already exists\n");
            } else if (strstr(msgrecv, "400 BAD REQUEST")) {
                printf("ERROR: date add_movie incomplete\n");
            } else if (strstr(msgrecv, "error")) {
                printf("ERROR: unknown error\n");
            } else {
                printf("SUCCES: movie added with success\n");
            }

            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);
            

        } else
        // --------------------- logout ---------------------
        if (strcmp(buff.data, "logout\n") == 0) {
            /*
                GET /api/v1/tema/user/logout

            */
            char *url = "/api/v1/tema/user/logout";

            message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);

            if (strstr(msgrecv, "User logged out successfully") != NULL) {
                printf("SUCCES: User logged out successfully\n");
            } else {
                printf("ERROR: couldn't log out the user\n");
            }

            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);

        } else 
        // -------------------- update_movie ----------------------
        if (strcmp(buff.data, "update_movie\n") == 0) {

            /*
                update_movie

                id=2
                title=New Title
                year=2009
                description=sci-fi
                rating=10.0

                Input
                update_movie
                id=2
                title=New Title
                year=2009
                description=sci-fi
                rating=10.0
                Output

                SUCCESS: Film actualizat
                            
                Ruta de acces:

                PUT /api/v1/tema/library/movies/:movieId
                Tip payload:

                application/json
                Payload:

                {
                    "title": String,
                    "year": Number,
                    "description": String,
                    "rating": Number
                }
                Observații:
                🍪 Trebuie să demonstraţi că aveţi acces la library! (JWT token)

                Erori tratate:
                👎 Fără acces library.
                👎 ID invalid.
                👎 Date invalide/incomplete.
            
            
            */
            printf("id=\n");
            char id[50];
            fgets(id, sizeof(id), stdin);
            replace_char(id, '\n', '\0');

            printf("title=\n");
            char title[50];
            fgets(title, sizeof(title), stdin);
            replace_char(title, '\n', '\0');

            printf("year=\n");
            char year[10];
            fgets(year, sizeof(year), stdin);
            replace_char(year, '\n', '\0');

            printf("description=\n");
            char description[500];
            fgets(description, sizeof(description), stdin);
            replace_char(description, '\n', '\0');

            printf("rating=\n");
            char rating[50];
            fgets(rating, sizeof(rating), stdin);
            replace_char(rating, '\n', '\0');

            double rat;
            sscanf(rating, "%lf", &rat);

            char url[100];
            sprintf(url, "/api/v1/tema/library/movies/%s", id);

            zero_matrix(matrix, matrix_names);

            strcpy(matrix[0], title);
            strcpy(matrix[1], year);
            strcpy(matrix[2], description);
            strcpy(matrix[3], rating);
            matrix[4][0] = '\0';

            strcpy(matrix_names[0], "title");
            strcpy(matrix_names[1], "year");
            strcpy(matrix_names[2], "description");
            strcpy(matrix_names[3], "rating");
            matrix_names[4][0] = '\0';

            char *json_string = json_builder(matrix, matrix_names);

            if (json_string == NULL) {
                fprintf(stderr, "Error: Failed to create JSON payload.\n");
            } else {
                message = compute_put_request(host, url, cookies, MAX_COOKIES, json_string);
                send_to_server(sockfd, message);
                json_free_serialized_string(json_string);
            }

            msgrecv = receive_from_server(sockfd);

            if (rat > 10.0 || rat < 0.0) {
                printf("ERROR: rating invalid\n");
                continue;
            }

            if (strstr(msgrecv, "Movie already exists")) {
                printf("ERROR: movie already exists\n");
            } else if (strstr(msgrecv, "400 BAD REQUEST")) {
                printf("ERROR: date update_movie incomplete\n");
            }  else if (strstr(msgrecv, "error")) {
                printf("ERROR: id invalid sau alte erori\n");
            } else {
                printf("SUCCES: movie updated with success\n");
            }

            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);
            

        } else 
        // --------------------- delete_movie ---------------------
        if (strcmp(buff.data, "delete_movie\n") == 0) {
            /*
                DELETE /api/v1/tema/library/movies/:movieId

                Input
                delete_movie
                id=10
                Output

                SUCCESS: Film șters cu succes

                Erori tratate:
                👎 Fără acces library.
                👎 ID invalid.
            */
            printf("id=\n");
            char id[50];
            fgets(id, sizeof(id), stdin);
            replace_char(id, '\n', '\0');

            char url[100];
            snprintf(url, sizeof(url), "/api/v1/tema/library/movies/%s", id);

            message = compute_delete_request(host, url, cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);
            if (strstr(msgrecv, "200 OK") != NULL) {
                printf("SUCCESS: Film șters\n");
            } else if (strstr(msgrecv, "UNAUTHORIZED") != NULL) {
                printf("ERROR: nu sunt permisiuni\n");
            } else {
                printf("ERROR: nu s-a gasit filmul\n");
            }

            print_messages(message, msgrecv);

            add_cookie(cookies, session_cookies, msgrecv);

        } else
        // --------------------- get_collections  ---------------------
        if (strcmp(buff.data, "get_collections\n") == 0) {
            /*
                GET /api/v1/tema/library/collections
                    Răspuns:

                    [
                    {
                        "id": Number,
                        "title": String,
                        "owner": String,
                        "movies": [
                        {
                            "id": Number,
                            "title": String
                        }
                        ]
                    }
                    ]

                Output
                    SUCCESS: Lista colecțiilor
                    #1: Colectie A
                    #2: Colectie B

            */
            char *url = "/api/v1/tema/library/collections";

            message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);

            char *content_ptr = strstr(msgrecv, "{\"collections");
            JSON_Value *root_value = json_parse_string(content_ptr);
            JSON_Object *root_object = json_value_get_object(root_value);
            JSON_Array *collections = json_object_get_array(root_object, "collections");

            size_t count = json_array_get_count(collections);

            if (strstr(msgrecv, "\"collections\":[") != NULL) {
                printf("SUCCESS: Lista colecțiilor\n");
                for (size_t i = 0; i < count; i++) {
                    JSON_Object *collection = json_array_get_object(collections, i);
                    int id = (int)json_object_get_number(collection, "id");
                    const char *title = json_object_get_string(collection, "title");

                    printf("#%d %s\n", id, title);
                }
            } else {
                printf("ERROR: serverul nu a intors lista colecțiilor\n");
            }


            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);

            json_value_free(root_value);


        } else
        // --------------------- get_collection  ---------------------
        if (strcmp(buff.data, "get_collection\n") == 0) {
            /*
                Ruta de acces:

                GET /api/v1/tema/library/collections/:collectionId
                Observații:
                🍪 Trebuie să demonstraţi că aveţi acces la library! (JWT token)

                e.g: /api/v1/tema/library/collections/123

                Răspuns:

                {
                "id": Number,
                "title": String,
                "owner": String,
                "movies": [
                    {
                    "id": Number,
                    "title": String
                    }
                ]
                }

                Input
                get_collection
                id=10
                Output
                SUCCESS: Detalii colecție
                title: Favorite Movies
                owner: test_user
                #3: Inception
                #5: Interstellar
            */
            char *url = "/api/v1/tema/library/collections/";
            printf("id=\n");
            char id[10];
            fgets(id, sizeof(id), stdin);
            replace_char(id, '\n', '\0');

            message = compute_get_request(host, url, id, cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);



            if (strstr(msgrecv, "\"id\"") != NULL) {
                // {"description":"test","id":75707,"rating":"6.0","title":"test","user_id":27291,"year":2000}
            /*
                    [
                    {
                        "id": Number,
                        "title": String,
                        "owner": String,
                        "movies": [
                        {
                            "id": Number,
                            "title": String
                        }
                        ]
                    }
                    ]
                    */
                char *content_ptr = strstr(msgrecv, "{\"id");
                JSON_Value *root_value = json_parse_string(content_ptr);
                JSON_Object *root_object = json_value_get_object(root_value);


                const char *owner = json_object_get_string(root_object, "owner");
                const char *title = json_object_get_string(root_object, "title");
                printf("SUCCESS: Detalii colecție\n"
                        "title: %s\n"
                        "owner: %s\n",
                        title, owner);

                JSON_Array *movies = json_object_get_array(root_object, "movies");
                int elems = json_array_get_count(movies);
                for (size_t i = 0; i < elems; i++) {
                    JSON_Object *movie = json_array_get_object(movies, i);
                    const char *movie_title = json_object_get_string(movie, "title");
                    int id = (int)json_object_get_number(movie, "id");
                    printf("#%d: %s\n", id, movie_title);
                }

                json_value_free(root_value);

            } else if (strstr(msgrecv, "Collection not found") != NULL) {
                 printf("ERROR: Collection not found, invalid id\n");
            } else {
                printf("ERROR: serverul zice ca nu am acces la Collections\n");
            }

            // print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);

        } else 
        // -------------------- add_collection ----------------------
        if (strcmp(buff.data, "add_collection\n") == 0) {
            /*
            POST /api/v1/tema/library/collections
            Tip payload:

            application/json
            Payload:

            {
            "title": String
            }
            Observații:
            🍪 Trebuie să demonstraţi că aveţi acces la library! (JWT token)
            ❗ Owner este utilizatorul curent.

            Erori tratate:
            👎 Fără acces library.
            👎 Date invalide/incomplete.

            Input
            add_collection
            title=Must See
            num_movies=2
            movie_id[0]=1
            movie_id[1]=3
            Output
            SUCCESS: Colecție adăugată

            */
            char *url = "/api/v1/tema/library/collections";
            printf("title=\n");
            char title[50];
            fgets(title, sizeof(title), stdin);
            replace_char(title, '\n', '\0');

            printf("num_movies=\n");
            char nm[10];
            fgets(nm, sizeof(nm), stdin);
            replace_char(nm, '\n', '\0');
            int num_movies;
            sscanf(nm, "%d", &num_movies);

            char id[10];
            // char movie_ids[200];
            // sprintf(movie_ids, "%s ", nm);
            // movie_ids[0] = '\0';
            char collection_id[100];
            // for (size_t i = 0; i < num_movies; i++) {
            //     memset(id, 0, sizeof(id));
            //     printf("movie_id[%ld]=\n", i);
            //     fgets(id, sizeof(id), stdin);
            //     replace_char(id, '\n', '\0');
            //     strcat(movie_ids, id);
            //     strcat(movie_ids, " ");
            // }

            char json_string[100];
            sprintf(json_string, "{\"title\":\"%s\"}", title);

            message = compute_post_request(host, url, "application/json",  json_string, cookies, MAX_COOKIES);
            send_to_server(sockfd, message);

            msgrecv = receive_from_server(sockfd);
            if (strstr(msgrecv, "Collection already exists")) {
                printf("ERROR: Collection already exists\n");
            } else if (strstr(msgrecv, "400 BAD REQUEST")) {
                printf("ERROR: datele add_collection sunt incomplete\n");
            } else {
                char *pin = strstr(msgrecv, "{\"id\":");
                char *pin2 = pin + 6;
                int j = 0;
                while(*(pin2 + j) != ',') j++;
                strncpy(collection_id, pin2, j);
                collection_id[j] = '\0';
                for (size_t k = 0; k < num_movies; k++) {
                    memset(id, 0, sizeof(id));
                    printf("movie_id[%ld]=\n", k);
                    fgets(id, sizeof(id), stdin);
                    replace_char(id, '\n', '\0');

                    add_movie_to_collection(collection_id, id, host, cookies, sockfd);
                }
                printf("SUCCESS: Colecție adăugată\n");

            }


            print_messages(message, msgrecv);
            add_cookie(cookies, session_cookies, msgrecv);
            

        } else 
        // --------------------- delete_collection ---------------------
        if (strcmp(buff.data, "delete_collection\n") == 0) {
            /*
                DELETE /api/v1/tema/library/collections/:collectionId
                Observații:
                🍪 Trebuie să demonstraţi că aveţi acces la library! (JWT token)
                ❗ Trebuie să fiți owner.

                Erori tratate:
                👎 Fără acces library.
                👎 Nu sunteți owner.
                👎 ID invalid.

                Input
                    delete_collection
                    id=10
                    Output
                        SUCCESS: Colecție ștearsă
            */
            printf("id=\n");
            char id[50];
            fgets(id, sizeof(id), stdin);
            replace_char(id, '\n', '\0');

            char url[100];
            snprintf(url, sizeof(url), "/api/v1/tema/library/collections/%s", id);

            message = compute_delete_request(host, url, cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);
            if (strstr(msgrecv, "200 OK") != NULL) {
                printf("SUCCESS: Colecție ștearsă\n");
            } else if (strstr(msgrecv, "UNAUTHORIZED") != NULL) {
                printf("ERROR: nu sunt permisiuni\n");
            } else {
                printf("ERROR: nu s-a gasit colecția\n");
            }

            print_messages(message, msgrecv);

            add_cookie(cookies, session_cookies, msgrecv);

        } else 
        // --------------------- delete_movie_from_collection ---------------------
        if (strcmp(buff.data, "delete_movie_from_collection\n") == 0) {
            /*
                Ruta de acces:

                DELETE /api/v1/tema/library/collections/:collectionId/movies/:movieId
                e.g: /api/v1/tema/library/collections/5/movies/123

                Observații:
                🍪 Trebuie să demonstraţi că aveţi acces la library! (JWT token)
                ❗Trebuie să fiți owner.
                ❗ Filmul trebuie să existe în colecție.

                Erori tratate:
                👎 Fără acces library.
                👎 Nu sunteți owner.
                👎 ID invalid.

                Input
                    delete_movie_from_collection
                    collection_id=10
                    movie_id=3
                    Output

                    SUCCESS: Film șters din colecție
            */
            printf("collection_id=\n");
            char collection_id[50];
            fgets(collection_id, sizeof(collection_id), stdin);
            replace_char(collection_id, '\n', '\0');

            printf("movie_id=\n");
            char id[50];
            fgets(id, sizeof(id), stdin);
            replace_char(id, '\n', '\0');

            char url[200];
            snprintf(url, sizeof(url), "/api/v1/tema/library/collections/%s/movies/%s", collection_id, id);

            message = compute_delete_request(host, url, cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            msgrecv = receive_from_server(sockfd);
            if (strstr(msgrecv, "200 OK") != NULL) {
                printf("SUCCESS: Film șters din colecție\n");
            } else if (strstr(msgrecv, "UNAUTHORIZED") != NULL) {
                printf("ERROR: nu sunt permisiuni\n");
            } else {
                printf("ERROR: nu s-a gasit colecția\n");
            }

            print_messages(message, msgrecv);

            add_cookie(cookies, session_cookies, msgrecv);

        } else 
        // -------------------- add_movie_to_collection ----------------------
        if (strcmp(buff.data, "add_movie_to_collection\n") == 0) {
            /*
                POST /api/v1/tema/library/collections/:collectionId/movies
                Tip payload:

                {
                "id": Number
                }
                Observații:
                🍪 Trebuie să demonstraţi că aveţi acces la library! (JWT token)
                ❗ Trebuie să fiți owner.

                Erori tratate:
                👎 Fără acces library.
                👎 Nu sunteți owner.
                👎 Date invalide/incomplete.

                Input
                add_movie_to_collection
                collection_id=10
                movie_id=3
                Output

                SUCCESS: Film adăugat în colecție

            */


            printf("collection_id=\n");
            char collection_id[50];
            fgets(collection_id, sizeof(collection_id), stdin);
            replace_char(collection_id, '\n', '\0');

            printf("movie_id=\n");
            char movie_id[50];
            fgets(movie_id, sizeof(movie_id), stdin);
            replace_char(movie_id, '\n', '\0');

            add_movie_to_collection(collection_id, movie_id, host, cookies, sockfd);

        }


        if (message != NULL)
            free(message);
        if(msgrecv != NULL)
            free(msgrecv);

        buffer_destroy(&buff);

    }

    close_connection(sockfd);
    return 0;
}