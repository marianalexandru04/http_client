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



char *json_builder(char* username, char* parola) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", parola);
    char *json_string = json_serialize_to_string(root_value);
    return json_string;
}
void login_admin(int sockfd,char* username, char* parola) {
    // 1. Construim JSON-ul
    // JSON_Value *root_value = json_value_init_object();
    // JSON_Object *root_object = json_value_get_object(root_value);
    // json_object_set_string(root_object, "username", username);
    // json_object_set_string(root_object, "password", parola);
    // char *json_string = json_serialize_to_string(root_value);

    char *json_string = json_builder(username, parola);

    // 2. Construim mesajul HTTP
    char message[4096];
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

    // printf("%s", message);
    // 3. Trimitem mesajul
    send_to_server(sockfd, message);

    // Cleanup
    json_free_serialized_string(json_string);
    json_value_free(root_value);
}
char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}
void add_user_and_password(User *users, char* username, char* password, char * usercount) {
    users[*usercount]->name = malloc(sizeof(char) * strlen(username));
    users[*usercount]->password = malloc(sizeof(char) * strlen(password));
    strcpy(users->name, username);
    strcpy(users->password, password);
    (*usercount) ++;

}
bool isUser(User *users, char* username, char * usercount) {
    for(size_t i = 0; i < (*usercount); i++) {
        if(strcmp(username, users[i]->name) == 0)
            return true;
    }
    return false;
}
typedef struct {
    char cookies[20][100];
    int count;
}Cookies;

typedef struct {
    char *name;
    char *password;
} User;
int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    Cookies cookies;
    cookies.count = 0;

    char usercount = 0;
    User *users;
    users = malloc(sizeof (User) * 50);
    

    setvbuf(stdout, NULL, _IONBF, 0);

    char * host_ip = "63.32.125.183";
    char * host = "63.32.125.183:8081";
    int portno = 8081;
    sockfd = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);

    char session_cookies[200];
    users

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


            compute_message(message, username);
            compute_message(message, password);
            login_admin(sockfd, username, password);

            char *msgrecv = receive_from_server(sockfd);

            if (strstr(msgrecv, "Admin logged in successfully")) {
                printf("SUCCESS: Admin autentificat cu succes\n");
                char * ptr = strstr(msgrecv, "Set-Cookie:");
                strcpy(cookies.cookies[cookies.count], ptr + 4);
                char *ptr2 = strchr(cookies.cookies[cookies.count],';');
                *ptr2 = '\0';
                cookies.count++;
                // *(ptr2 + 1) = '\0';
                // printf("%s\n", cookies.cookies[cookies.count]);

            } else {
                printf("credentialele nu se potrivesc\n");
            }

            
            // printf("%s", msgrecv);
            // fflush(stdout);

        } else if (strcmp(buff.data, "login\n") == 0) {
            printf("test\n");
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


            add_user_and_password(users, username, password, &usercount);




            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            char *json_string = json_serialize_to_string(root_value);

            compute_message(message, "POST /api/v1/tema/admin/users HTTP/1.1");
            compute_message(message, "Host: 63.32.125.183:8081");
            compute_message(message, "Content-Type: application/json");
            char tmp[250];
            sprintf(tmp, "Content-Length: %ld", strlen(json_string));
            compute_message(message, tmp);
            sprintf(tmp, "Authorization: Bearer %s", cookies.cookies[0]);
            compute_message(message, cookies.cookies[0]);
            compute_message(message, "Connection: keep-alive");
            compute_message(message, "");
            sprintf(tmp, "%s", json_string);
            compute_message(message, tmp);
            if (!isUser(users, username, &usercount))
                send_to_server(sockfd, message);

            json_free_serialized_string(json_string);
            json_value_free(root_value);



            printf("\n%s\n", message);
            char *msgrecv = receive_from_server(sockfd);
            

        } else 
        // --------------------- get_users ---------------------
        if (strcmp(buff.data, "get_users\n") == 0) {


            compute_message(message, "GET /api/v1/tema/admin/users HTTP/1.1");
            compute_message(message, "Host: 63.32.125.183:8081");
            char tmp[250];
            compute_message(message, tmp);
            sprintf(tmp, "Authorization: Bearer %s", cookies.cookies[0]);
            compute_message(message, cookies.cookies[0]);
            compute_message(message, "Connection: keep-alive");
            compute_message(message, "");
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


            char * message = compute_delete_request(host, url, cookies.cookies, cookies.count);

            send_to_server(sockfd, message);
            char *msgrecv = receive_from_server(sockfd);


        } else
        // --------------------- logout_admin ---------------------
        if (strcmp(buff.data, "logout_admin\n") == 0) {
            /*
                GET /api/v1/tema/admin/logout

            */
            char *url = "/api/v1/tema/admin/logout";


            char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);

            send_to_server(sockfd, message);
            char *msgrecv = receive_from_server(sockfd);


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


            char * message = compute_get_request(host, url, NULL,  cookies.cookies, cookies.count);

            send_to_server(sockfd, message);
            char *msgrecv = receive_from_server(sockfd);


        }


    
        free(response);
        free(message);
        buffer_destroy(&buff);

    }

    close_connection(sockfd);
    return 0;
}
