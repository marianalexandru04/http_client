// #include <stdio.h>      /* printf, sprintf */
// #include <stdlib.h>     /* exit, atoi, malloc, free */
// #include <unistd.h>     /* read, write, close */
// #include <string.h>     /* memcpy, memset */
// #include <sys/socket.h> /* socket, connect */
// #include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
// #include <netdb.h>      /* struct hostent, gethostbyname */
// #include <arpa/inet.h>
// #include "helpers.h"
// #include "requests.h"
// #include "buffer.h"
// #include "parson.h"
// #include <stdbool.h>

// #define MAX_COOKIES 10
// typedef struct {
//     char cookies[20][100];
//     int count;
// }Cookies;

// typedef struct {
//     char *name;
//     char *password;
// } User;


// char *json_builder(char types[][50], char names[][50]) {
//     JSON_Value *root_value = json_value_init_object();
//     if (root_value == NULL) {
//         fprintf(stderr, "Error: json_value_init_object failed in json_builder\n");
//         return NULL;
//     }
//     JSON_Object *root_object = json_value_get_object(root_value);

//     int i = 0;
//     while (types[i][0] != '\0') {
//         if(strcmp(names[i], "year") == 0 || strcmp(names[i], "rating") == 0) {
//             double nr = strtod(types[i], NULL);
//             json_object_set_number(root_object, names[i], nr);
//         } else {
//             json_object_set_string(root_object, names[i], types[i]);
//         }
//         i++;
//     }
//     char *json_string = json_serialize_to_string(root_value);
//     json_value_free(root_value); // Free the JSON_Value structure itself
//     return json_string;
// }
// void zero_matrix(char types[][50], char names[][50]) {
//     int i = 0;
//     while (types[i][0] != '\0') {
//         memset(types[i], 0, 50);
//         i++;
//     }
//     int j = 0;
//     while (names[j][0] != '\0') {
//         memset(names[j], 0, 50);
//         j++;
//     }
// }
// char* replace_char(char* str, char find, int replace){
//     char *current_pos = strchr(str,find);
//     while (current_pos) {
//         *current_pos = replace;
//         current_pos = strchr(current_pos,find);
//     }
//     return str;
// }
// void add_user_and_password(User **users, char* username, char* password, int * usercount) {
//     users[*usercount]->name = malloc(sizeof(char) * strlen(username));
//     users[*usercount]->password = malloc(sizeof(char) * strlen(password));
//     strcpy(users[*usercount]->name, username);
//     strcpy(users[*usercount]->password, password);
//     (*usercount) ++;

// }
// bool isUser(User **users, char* username, int * usercount) {
//     for(size_t i = 0; i < (*usercount); i++) {
//         if(strcmp(username, users[i]->name) == 0)
//             return true;
//     }
//     return false;
// }
// void add_cookie(char  ** cookies, char * cookie) {
//     for(int i = 0; i < MAX_COOKIES; i++) { // MAX_COOKIES is 10
//         if(cookies[i] == NULL) {
//             cookies[i] = malloc(strlen(cookie) + 1); // Corrected allocation size
//             if (cookies[i] == NULL) {
//                 perror("Failed to allocate memory for cookie");
//                 return;
//             }
//             strcpy(cookies[i], cookie);
//             return;
//         }
//     }
//     fprintf(stderr, "No space to add new cookie. Cookie array full.\n");
// }
// int main(int argc, char *argv[])
// {
//     char *message;
//     char *response;
//     int sockfd;

//     // Cookies cookies;
//     // cookies.count = 0;
//     char **cookies = calloc(10, sizeof(char*));
//     cookies[0] = calloc(500, sizeof(char));
//     cookies[1] = calloc(500, sizeof(char));

//     // int usercount = 0;
//     // User **users;
//     // users = malloc(sizeof(User*) * 50);
    

//     setvbuf(stdout, NULL, _IONBF, 0);

//     char * host_ip = "63.32.125.183";
//     char * host = "63.32.125.183:8081";
//     int portno = 8081;
//     sockfd = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);

//     char session_cookies[500];
    
    
//     char matrix[6][50];
//     matrix[0][0] = '\0';
//     char matrix_names[6][50];
//     matrix_names[0][0] = '\0';

//     // int ok = 0;
    

//     char stdincmd[200];
//     while (1) {
        
//         message = (char*)calloc(1000, sizeof(char));
//         response = (char*)calloc(1000, sizeof(char));
//         buffer buff = buffer_init();
//         memset(stdincmd, 0, sizeof(stdincmd));
//         fgets(stdincmd, sizeof(stdincmd), stdin);
//         buffer_add(&buff, stdincmd, sizeof(stdincmd));

//         if (strcmp(buff.data, "exit\n") == 0) {
//             free(message);
//             free(response);
//             buffer_destroy(&buff);
//             break;
//         } else 
//         // -------------------- login admin----------------------
//         if (strcmp(buff.data, "login_admin\n") == 0) {
//             printf("username=\n");
//             char username[50];
//             fgets(username, sizeof(username), stdin);
//             replace_char(username, '\n', '\0');

//             printf("password=\n");
//             char password[50];
//             fgets(password, sizeof(password), stdin);
//             replace_char(password, '\n', '\0');

//             zero_matrix(matrix, matrix_names);

//             strcpy(matrix[0], username);
//             strcpy(matrix[1], password);
//             matrix[2][0] = '\0';
//             strcpy(matrix_names[0], "username");
//             strcpy(matrix_names[1], "password");
//             matrix_names[2][0] = '\0';

//             char *json_string = json_builder(matrix, matrix_names);

//             // char message[1000];
//             sprintf(message,
//                 "POST /api/v1/tema/admin/login HTTP/1.1\r\n"
//                 "Host: 63.32.125.183:8081\r\n"
//                 "Content-Type: application/json\r\n"
//                 "Content-Length: %ld\r\n"
//                 "Connection: keep-alive\r\n"
//                 "\r\n"
//                 "%s",
//                 strlen(json_string), json_string
//             );

//             send_to_server(sockfd, message);


//             char *msgrecv = receive_from_server(sockfd);

//             if (strstr(msgrecv, "Admin logged in successfully")) {
//                 printf("SUCCESS: Admin autentificat cu succes\n");
//                 char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 strcpy(session_cookies, ptr + 4);
//                 char *ptr2 = strchr(session_cookies,';');
//                 *ptr2 = '\0';
//                 strcpy(cookies[0], session_cookies);
//                 printf("%s\n", cookies[0]);

//             } else {
//                 printf("ERROR: admin already logged in\n");
//             }

            
//             printf("%s", msgrecv);
//             // fflush(stdout);
//             free(msgrecv);

//         } else 
//         // -------------------- add user----------------------
//         if (strcmp(buff.data, "add_user\n") == 0 || strcmp(buff.data, "add_user") == 0) {
//             printf("username=\n");
//             char username[50];
//             fgets(username, sizeof(username), stdin);
//             replace_char(username, '\n', '\0');
//             printf("password=\n");
//             char password[50];
//             fgets(password, sizeof(password), stdin);
//             replace_char(password, '\n', '\0');

//             zero_matrix(matrix, matrix_names);
//             strcpy(matrix[0], username);
//             strcpy(matrix[1], password);
//             matrix[2][0] = '\0';
//             strcpy(matrix_names[0], "username");
//             strcpy(matrix_names[1], "password");
//             matrix_names[2][0] = '\0';

//             char *json_string = json_builder(matrix, matrix_names);

//             if (json_string == NULL) {
//                 printf("Error: Failed to create JSON payload for add_user.\n");
//             } else {
//                 compute_message(message, "POST /api/v1/tema/admin/users HTTP/1.1");
//                 compute_message(message, "Host: 63.32.125.183:8081");
//                 compute_message(message, "Content-Type: application/json");
//                 char tmp[250];
//                 sprintf(tmp, "Content-Length: %ld", strlen(json_string));
//                 compute_message(message, tmp);

//                 if (cookies[0] != NULL) {
//                     sprintf(tmp, "%s", cookies[0]);
//                     compute_message(message, tmp);
//                 } else {
//                     printf("Warning: No authorization cookie found for add_user. Request may fail.\n");
//                     // Depending on API requirements, you might not want to send the request
//                     // or add a specific header indicating no authorization.
//                 }
//                 // compute_message(message, cookies[0]); // REMOVED: This line was incorrect.

//                 compute_message(message, "Connection: keep-alive");
//                 compute_message(message, ""); // Empty line before body
//                 compute_message(message, json_string); // Add JSON body directly

//                 // if (!isUser(users, username, &usercount)) // This logic was commented out
//                 send_to_server(sockfd, message);
//                 char *msgrecv = receive_from_server(sockfd);
//                 printf("%s\n", msgrecv);
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);
//                 json_free_serialized_string(json_string);
//             }
//             // json_value_free(myroot); // Removed



//             char *msgrecv = receive_from_server(sockfd);
//             if (strstr(msgrecv, "User already exists"))
//                 printf("ERROR: user already exists\n");
//             printf("\n%s\n", msgrecv);
//             free(msgrecv);

//         } else 
//         // --------------------- get_users ---------------------
//         if (strcmp(stdincmd, "get_users\n") == 0 || strcmp(buff.data, "get_users") == 0) {


//             compute_message(message, "GET /api/v1/tema/admin/users HTTP/1.1");
//             compute_message(message, "Host: 63.32.125.183:8081");
//             // char tmp[250];
//             // compute_message(message, tmp);
//             // sprintf(tmp, "%s", cookies[0]);
//             compute_message(message, cookies[0]);
//             compute_message(message, "Connection: keep-alive");
//             compute_message(message, "");
//             printf("%s\n", message);
//             send_to_server(sockfd, message);

//             char *msgrecv = receive_from_server(sockfd);
//             printf("%s\n", msgrecv);
//             char *content_ptr = strstr(msgrecv, "{\"users");
//             JSON_Value *root_value = json_parse_string(content_ptr);
//             JSON_Object *root_object = json_value_get_object(root_value);
//             JSON_Array *users = json_object_get_array(root_object, "users");

//             size_t count = json_array_get_count(users);

//             printf("SUCCESS: Lista utilizatorilor\n");
//             for (size_t i = 0; i < count; i++) {
//                 JSON_Object *user = json_array_get_object(users, i);
//                 int id = (int)json_object_get_number(user, "id");
//                 const char *username = json_object_get_string(user, "username");
//                 const char *password = json_object_get_string(user, "password");

//                 printf("#%d %s:%s\n", id, username, password);
//             }
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);
//             json_value_free(root_value);
//             free(msgrecv);
//         } else 
//         // --------------------- delete_user ---------------------
//         if (strcmp(buff.data, "delete_user\n") == 0) {
//             /*
//                 char * host_ip = "63.32.125.183";
//                 int portno = 8081;
//             */
//             printf("username=\n");
//             char username[50];
//             fgets(username, sizeof(username), stdin);
//             replace_char(username, '\n', '\0');

//             char url[100];
//             snprintf(url, sizeof(url), "/api/v1/tema/admin/users/%s", username);


//             char * message = compute_delete_request(host, url, cookies, MAX_COOKIES);

//             send_to_server(sockfd, message);
//             char *msgrecv = receive_from_server(sockfd);
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);

//         } else
//         // --------------------- logout_admin ---------------------
//         if (strcmp(buff.data, "logout_admin\n") == 0) {
//             /*
//                 GET /api/v1/tema/admin/logout

//             */
//             char *url = "/api/v1/tema/admin/logout";


//             // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
//             char * message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

//             send_to_server(sockfd, message);
//             char *msgrecv = receive_from_server(sockfd);
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);

//         } else
//         // --------------------- login ---------------------
//         if (strcmp(buff.data, "login\n") == 0) {
//             /*
//                 POST /api/v1/tema/user/login
//                 Tip payload:

//                 application/json
//                 Payload:

//                 {
//                 "admin_username": String,
//                 "username": String,
//                 "password": String
//                 }

//             */
//             char *url = "/api/v1/tema/user/login";

//             printf("admin_username=\n");
//             char admin_username[50];
//             fgets(admin_username, sizeof(admin_username), stdin);
//             replace_char(admin_username, '\n', '\0');

//             printf("username=\n");
//             char username[50];
//             fgets(username, sizeof(username), stdin);
//             replace_char(username, '\n', '\0');

//             printf("password=\n");
//             char password[50];
//             fgets(password, sizeof(password), stdin);
//             replace_char(password, '\n', '\0');

//             zero_matrix(matrix, matrix_names);

//             strcpy(matrix[0], username);
//             strcpy(matrix[1], password);
//             strcpy(matrix[2], admin_username);
//             matrix[3][0] = '\0';

//             strcpy(matrix_names[0], "username");
//             strcpy(matrix_names[1], "password");
//             strcpy(matrix_names[2], "admin_username");
//             matrix_names[3][0] = '\0';

//             char *json_string = json_builder(matrix, matrix_names);
//             if (json_string == NULL) {
//                 fprintf(stderr, "Error: Failed to create JSON payload for user login.\n");
//             } else {
//                 char * message = compute_post_request(host, url, "application/json",  json_string, cookies, MAX_COOKIES);
//                 // json_free_serialized_string(json_string); // compute_post_request likely uses it, free after send
//                 // json_value_free(myroot); // Removed
//                 send_to_server(sockfd, message);
//                 free(message); // Assuming compute_post_request allocates message
//                 json_free_serialized_string(json_string); // Free json_string here
//             }
//             char *msgrecv = receive_from_server(sockfd);
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);

//         } else
//         // --------------------- get_access  ---------------------
//         if (strcmp(buff.data, "get_access\n") == 0) {
//             /*
//                 GET /api/v1/tema/library/access
//                 SUCCESS: Token JWT primit

//             */
//             char *url = "/api/v1/tema/library/access";

//             // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
//             char * message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

//             send_to_server(sockfd, message);
//             char *msgrecv = receive_from_server(sockfd);
//             // printf("%s\n", message);

//             // printf("%s\n", msgrecv);

//             printf("SUCCESS: Token JWT primit\n");

//             char * ptr = strstr(msgrecv, "\"token\":\"");
//             strcpy(session_cookies, ptr + 9);
//             char *ptr2 = strchr(session_cookies,'"');
//             *ptr2 = '\0';
//             // add_cookie(cookies, session_cookies);
//             // if(realloc(cookies[1], strlen(session_cookies) + 1) == NULL) {
//             //     printf("eroare realloc\n");
//             // }
//             strcpy(cookies[1], session_cookies);
//                 // char * ptr21 = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr21 + 4);
//                 // char *ptr22 = strchr(session_cookies,';');
//                 // *ptr22 = '\0';
//                 // strcpy(cookies[0], session_cookies);


//         } else
//         // --------------------- get_movies  ---------------------
//         if (strcmp(buff.data, "get_movies\n") == 0) {
//             /*
//                 GET /api/v1/tema/library/movies
//                 SUCCESS: Token JWT primit

//             */
//             char *url = "/api/v1/tema/library/movies";


//             // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
//             char * message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);
//             printf("%s\n", message);

//             send_to_server(sockfd, message);
//             char *msgrecv = receive_from_server(sockfd);

//             printf("%s", msgrecv);
//             // printf("SUCCESS: Token JWT primit\n");
//             // if (strstr(msgrecv, "Current user not found")) {
//                 // printf("ERROR: \n");
//             // } else {
//             //     char *content_ptr = strstr(msgrecv, "{\"movies");
//             //     JSON_Value *root_value = json_parse_string(content_ptr);
//             //     JSON_Object *root_object = json_value_get_object(root_value);
//             //     JSON_Array *movies = json_object_get_array(root_object, "movies");

//             //     size_t count = json_array_get_count(movies);

//             //     printf("SUCCESS: Lista filmelor\n");
//             //     for (size_t i = 0; i < count; i++) {
//             //         JSON_Object *user = json_array_get_object(movies, i);
//             //         int id = (int)json_object_get_number(user, "id");
//             //         const char *title = json_object_get_string(user, "title");

//             //         printf("#%d %s\n", id, title);
//             //     }

//             //     json_value_free(root_value);
//             // }
//             char *content_ptr = strstr(msgrecv, "{\"movies");
//             JSON_Value *root_value = json_parse_string(content_ptr);
//             JSON_Object *root_object = json_value_get_object(root_value);
//             JSON_Array *movies = json_object_get_array(root_object, "movies");

//             size_t count = json_array_get_count(movies);

//             printf("SUCCESS: Lista filmelor\n");
//             for (size_t i = 0; i < count; i++) {
//                 JSON_Object *user = json_array_get_object(movies, i);
//                 int id = (int)json_object_get_number(user, "id");
//                 const char *title = json_object_get_string(user, "title");

//                 printf("#%d %s\n", id, title);
//             }

//             json_value_free(root_value);
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);

//         } else
//         // --------------------- get_movie  ---------------------
//         if (strcmp(buff.data, "get_movie\n") == 0) {
//             /*
//                 GET /api/v1/tema/library/movies/:movieId
//                 {
//                     "id": Number,
//                     "title": String,
//                     "year": Number,
//                     "description": String,
//                     "rating": Number
//                 }


//             */
//             char *url = "/api/v1/tema/library/movies/";
//             printf("id=\n");
//             char id[10];
//             fgets(id, sizeof(id), stdin);
//             replace_char(id, '\n', '\0');
//             // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
//             char * message = compute_get_request(host, url, id,  cookies, MAX_COOKIES);

//             send_to_server(sockfd, message);
//             char *msgrecv = receive_from_server(sockfd);
//             printf("%s\n", message);

//             printf("%s\n", msgrecv);
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);
//         } else 
//         // -------------------- add_movie ----------------------
//         if (strcmp(buff.data, "add_movie\n") == 0) {
//             char *url = "/api/v1/tema/library/movies";
//             printf("title=\n");
//             char title[50];
//             fgets(title, sizeof(title), stdin);
//             replace_char(title, '\n', '\0');

//             printf("year=\n");
//             char year[10];
//             fgets(year, sizeof(year), stdin);
//             replace_char(year, '\n', '\0');

//             printf("description=\n");
//             char description[50];
//             fgets(description, sizeof(description), stdin);
//             replace_char(description, '\n', '\0');

//             printf("rating=\n");
//             char rating[10];
//             fgets(rating, sizeof(rating), stdin);
//             replace_char(rating, '\n', '\0');

//             zero_matrix(matrix, matrix_names);

//             strcpy(matrix[0], title);
//             strcpy(matrix[1], year);
//             strcpy(matrix[2], description);
//             strcpy(matrix[3], rating);
//             matrix[4][0] = '\0';

//             strcpy(matrix_names[0], "title");
//             strcpy(matrix_names[1], "year");
//             strcpy(matrix_names[2], "description");
//             strcpy(matrix_names[3], "rating");
//             matrix_names[4][0] = '\0';

//             char *json_string = json_builder(matrix, matrix_names);
//             // POST /api/v1/tema/library/movies

//             // if (json_string == NULL) {
//             //     fprintf(stderr, "Error: Failed to create JSON payload for add_movie.\n");
//             // } else {


//             char * message = compute_post_request(host, url, "application/json",  json_string, cookies, MAX_COOKIES);


//             // if (!isUser(users, username, &usercount)) // This logic was commented out
//             send_to_server(sockfd, message);

//             json_free_serialized_string(json_string);
//             // }
//             // json_value_free(myroot); // Removed


//             printf("\n%s\n", message);
//             char *msgrecv = receive_from_server(sockfd);
//             if (strstr(msgrecv, "User already exists"))
//                 printf("ERROR: user already exists\n");
//             printf("\n%s\n", msgrecv);
//                 // char * ptr = strstr(msgrecv, "Set-Cookie:");
//                 // strcpy(session_cookies, ptr + 4);
//                 // char *ptr2 = strchr(session_cookies,';');
//                 // *ptr2 = '\0';
//                 // strcpy(cookies[0], session_cookies);
//         }


    
//         free(response);
//         free(message);
//         buffer_destroy(&buff);

//     }

//     close_connection(sockfd);
//     return 0;
// }












































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


// char *json_builder(char types[][50], char names[][50]) {
//     JSON_Value *root_value = json_value_init_object();
//     if (root_value == NULL) {
//         fprintf(stderr, "Error: json_value_init_object failed in json_builder\\n");
//         return NULL;
//     }
//     JSON_Object *root_object = json_value_get_object(root_value);

//     int i = 0;
//     while (types[i][0] != '\0') {
//         json_object_set_string(root_object, names[i], types[i]);
//         i++;
//     }
//     char *json_string = json_serialize_to_string(root_value);
//     json_value_free(root_value); // Free the JSON_Value structure itself
//     return json_string;
// }

char *json_builder(char types[][50], char names[][50]) {
    JSON_Value *root_value = json_value_init_object();
    if (root_value == NULL) {
        fprintf(stderr, "Error: json_value_init_object failed in json_builder\n");
        return NULL;
    }
    JSON_Object *root_object = json_value_get_object(root_value);

    int i = 0;
    while (types[i][0] != '\0') {
        if(strcmp(names[i], "year") == 0 || strcmp(names[i], "rating") == 0) {
            double nr = strtod(types[i], NULL);
            json_object_set_number(root_object, names[i], nr);
        } else {
            json_object_set_string(root_object, names[i], types[i]);
        }
        i++;
    }
    char *json_string = json_serialize_to_string(root_value);
    json_value_free(root_value); // Free the JSON_Value structure itself
    return json_string;
}


void zero_matrix(char types[][50], char names[][50]) {
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
void add_cookie(char  ** cookies, char * cookie) {
    for(int i = 0; i < MAX_COOKIES; i++) { // MAX_COOKIES is 10
        if(cookies[i] == NULL) {
            cookies[i] = malloc(strlen(cookie) + 1); // Corrected allocation size
            if (cookies[i] == NULL) {
                perror("Failed to allocate memory for cookie");
                return;
            }
            strcpy(cookies[i], cookie);
            return;
        }
    }
    fprintf(stderr, "No space to add new cookie. Cookie array full.\\n");
}
int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    // Cookies cookies;
    // cookies.count = 0;
    char **cookies = calloc(10, sizeof(char*));
    cookies[0] = calloc(200, sizeof(char));
    cookies[1] = calloc(200, sizeof(char));
    cookies[1][0] = '\0';

    // int usercount = 0;
    // User **users;
    // users = malloc(sizeof(User*) * 50);
    

    setvbuf(stdout, NULL, _IONBF, 0);

    char * host_ip = "63.32.125.183";
    char * host = "63.32.125.183:8081";
    int portno = 8081;
    sockfd = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);

    char session_cookies[200];
    
    
    char matrix[6][50];
    matrix[0][0] = '\0';
    char matrix_names[6][50];
    matrix_names[0][0] = '\0';

    int ok = 0;
    

    char stdincmd[1000];
    while (1) {
        message = (char*)calloc(1000, sizeof(char));
        response = (char*)calloc(1000, sizeof(char));
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

            sprintf(message,
                "POST /api/v1/tema/admin/login HTTP/1.1\r\n"
                "Host: 63.32.125.183:8081\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %ld\r\n"
                "Connection: keep-alive\r\n"
                "\r\n"
                "%s",
                strlen(json_string), json_string
            );

            send_to_server(sockfd, message);
            printf("%s\n", message);

            char *msgrecv = receive_from_server(sockfd);
            printf("%s\n", msgrecv);

            if (strstr(msgrecv, "Admin logged in successfully")) {
                printf("SUCCESS: Admin autentificat cu succes\n");
                char * ptr = strstr(msgrecv, "Set-Cookie:");
                strcpy(session_cookies, ptr + 4);
                char *ptr2 = strchr(session_cookies,';');
                *ptr2 = '\0';
                strcpy(cookies[0], session_cookies);

                printf("%s\n", cookies[0]);

            } else {
                printf("ERROR: admin already logged in\n");
            }

            
            // printf("%s", msgrecv);
            // fflush(stdout);

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
                compute_message(message, "POST /api/v1/tema/admin/users HTTP/1.1");
                compute_message(message, "Host: 63.32.125.183:8081");
                compute_message(message, "Content-Type: application/json");
                char tmp[250];
                sprintf(tmp, "Content-Length: %ld", strlen(json_string));
                compute_message(message, tmp);

                if (cookies[0] != NULL) {
                    sprintf(tmp, "%s", cookies[0]);
                    compute_message(message, tmp);
                } else {
                    fprintf(stderr, "Warning: No authorization cookie found for add_user. Request may fail.\\n");
                    // Depending on API requirements, you might not want to send the request
                    // or add a specific header indicating no authorization.
                }
                // compute_message(message, cookies[0]); // REMOVED: This line was incorrect.

                compute_message(message, "Connection: keep-alive");
                compute_message(message, ""); // Empty line before body
                compute_message(message, json_string); // Add JSON body directly

                // if (!isUser(users, username, &usercount)) // This logic was commented out
                send_to_server(sockfd, message);

                json_free_serialized_string(json_string);
            }
            // json_value_free(myroot); // Removed


            printf("%s\n", message);
            char *msgrecv = receive_from_server(sockfd);
            if (strstr(msgrecv, "User already exists"))
                printf("ERROR: user already exists\n");
            printf("\n%s\n", msgrecv);

        } else 
        // --------------------- get_users ---------------------
        if (strcmp(buff.data, "get_users\n") == 0) {


            compute_message(message, "GET /api/v1/tema/admin/users HTTP/1.1");
            compute_message(message, "Host: 63.32.125.183:8081");
            // char tmp[250];
            // compute_message(message, tmp);
            // sprintf(tmp, "%s", cookies[0]);
            compute_message(message, cookies[0]);
            compute_message(message, "Connection: keep-alive");
            compute_message(message, "");

            // sleep(1);
            send_to_server(sockfd, message);

            char *msgrecv = receive_from_server(sockfd);

            char *content_ptr = strstr(msgrecv, "{\"users");
            JSON_Value *root_value = json_parse_string(content_ptr);
            JSON_Object *root_object = json_value_get_object(root_value);
            JSON_Array *users = json_object_get_array(root_object, "users");

            size_t count = json_array_get_count(users);

            printf("SUCCESS: Lista utilizatorilor\n");
            for (size_t i = 0; i < count; i++) {
                JSON_Object *user = json_array_get_object(users, i);
                int id = (int)json_object_get_number(user, "id");
                const char *username = json_object_get_string(user, "username");
                const char *password = json_object_get_string(user, "password");

                printf("#%d %s:%s\n", id, username, password);
            }
            printf("%s\n", message);
            printf("%s\n", msgrecv);

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
            char *msgrecv = receive_from_server(sockfd);
            printf("%s\n", message);
            printf("%s\n", msgrecv);


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
            char *msgrecv = receive_from_server(sockfd);
            printf("%s\n", message);
            printf("%s\n", msgrecv);

            char * ptr = strstr(msgrecv, "Set-Cookie:");
            if (ptr != NULL) {
                strcpy(session_cookies, ptr + 4);
                char *ptr2 = strchr(session_cookies,';');
                *ptr2 = '\0';
                strcpy(cookies[0], session_cookies);
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
                // json_free_serialized_string(json_string); // compute_post_request likely uses it, free after send
                // json_value_free(myroot); // Removed
                send_to_server(sockfd, message);
                // free(message); // Assuming compute_post_request allocates message
                json_free_serialized_string(json_string); // Free json_string here
            }
            char *msgrecv = receive_from_server(sockfd);
            printf("%s\n", message);
            printf("%s\n", msgrecv);

            char * ptr = strstr(msgrecv, "Set-Cookie:");
            if (ptr != NULL) {
                strcpy(session_cookies, ptr + 4);
                char *ptr2 = strchr(session_cookies,';');
                *ptr2 = '\0';
                strcpy(cookies[0], session_cookies);
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
            char *msgrecv = receive_from_server(sockfd);
            printf("%s\n", message);

            printf("%s\n", msgrecv);

            printf("SUCCESS: Token JWT primit\n");

            char * ptr = strstr(msgrecv, "\"token\":\"");
            strcpy(session_cookies, ptr + 9);
            char *ptr2 = strchr(session_cookies,'"');
            *ptr2 = '\0';
            // add_cookie(cookies, session_cookies);
            // if(realloc(cookies[1], strlen(session_cookies) + 1) == NULL) {
            //     printf("eroare realloc\n");
            // }
            strcpy(cookies[1], session_cookies);



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
            printf("%s\n", message);

            send_to_server(sockfd, message);
            char *msgrecv = receive_from_server(sockfd);

            printf("%s", msgrecv);

            char *content_ptr = strstr(msgrecv, "{\"movies");
            JSON_Value *root_value = json_parse_string(content_ptr);
            JSON_Object *root_object = json_value_get_object(root_value);
            JSON_Array *movies = json_object_get_array(root_object, "movies");

            size_t count = json_array_get_count(movies);

            printf("SUCCESS: Lista filmelor\n");
            for (size_t i = 0; i < count; i++) {
                JSON_Object *user = json_array_get_object(movies, i);
                int id = (int)json_object_get_number(user, "id");
                const char *title = json_object_get_string(user, "title");

                printf("#%d %s\n", id, title);
            }
            // char * ptr = strstr(msgrecv, "Set-Cookie:");
            // strcpy(session_cookies, ptr + 4);
            // char *ptr2 = strchr(session_cookies,';');
            // *ptr2 = '\0';
            // strcpy(cookies[0], session_cookies);

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


            */
            char *url = "/api/v1/tema/library/movies/";
            printf("id=\n");
            char id[10];
            fgets(id, sizeof(id), stdin);
            replace_char(id, '\n', '\0');
            // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
            message = compute_get_request(host, url, id,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            char *msgrecv = receive_from_server(sockfd);
            printf("%s\n", message);

            printf("%s\n", msgrecv);

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
            char description[50];
            fgets(description, sizeof(description), stdin);
            replace_char(description, '\n', '\0');

            printf("rating=\n");
            char rating[10];
            fgets(rating, sizeof(rating), stdin);
            replace_char(rating, '\n', '\0');

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
            // POST /api/v1/tema/library/movies

            if (json_string == NULL) {
                fprintf(stderr, "Error: Failed to create JSON payload for add_movie.\\n");
            } else {


                message = compute_post_request(host, url, "application/json",  json_string, cookies, MAX_COOKIES);


                // if (!isUser(users, username, &usercount)) // This logic was commented out
                send_to_server(sockfd, message);

                json_free_serialized_string(json_string);
            }
            // json_value_free(myroot); // Removed


            // printf("\\n%s\\n", message);
            char *msgrecv = receive_from_server(sockfd);
            if (strstr(msgrecv, "User already exists"))
                printf("ERROR: user already exists\n");
            printf("%s\n", message);
            printf("%s\n", msgrecv);
            

        } else
        // --------------------- logout ---------------------
        if (strcmp(buff.data, "logout\n") == 0) {
            /*
                GET /api/v1/tema/user/logout

            */
            char *url = "/api/v1/tema/user/logout";


            // char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);
            message = compute_get_request(host, url, NULL,  cookies, MAX_COOKIES);

            send_to_server(sockfd, message);
            char *msgrecv = receive_from_server(sockfd);
            printf("%s\n", message);
            printf("%s\n", msgrecv);
            printf("SUCCESS: Utilizator delogat\n");

            char * ptr = strstr(msgrecv, "Set-Cookie:");
            if (ptr != NULL) {
                strcpy(session_cookies, ptr + 4);
                char *ptr2 = strchr(session_cookies,';');
                *ptr2 = '\0';
                strcpy(cookies[0], session_cookies);
            }
            
        }


    
        free(response);
        free(message);
        buffer_destroy(&buff);

    }

    close_connection(sockfd);
    return 0;
}