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



void login_admin(int sockfd,char* username, char* parola) {
    // 1. Construim JSON-ul
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", parola);
    char *json_string = json_serialize_to_string(root_value);

    // 2. Construim mesajul HTTP
    char message[4096];
    sprintf(message,
        "POST /api/v1/tema/admin/login HTTP/1.1\r\n"
        "Host: 63.32.125.183:8081\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(json_string), json_string
    );

    printf("%s", message);
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


int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

        
    // Ex 1.1: GET dummy from main server
    // Ex 1.2: POST dummy and print response from main server
    // Ex 2: Login into main server
    // Ex 3: GET weather key from main server
    // Ex 4: GET weather data from OpenWeather API
    // Ex 5: POST weather data for verification to main server
    // Ex 6: Logout from main server

    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!


    /*
    Descriere Server
        Datele de conectare
        HOST: 63.32.125.183 PORT: 8081

        Mod de funcționare
        Serverul va permite efectuarea următoarelor acțiuni:

        4.1 Autentificare admin
        Ruta de acces:

        POST /api/v1/tema/admin/login
        Tip payload:

        application/json
        Payload:

        {
        "username": String,
        "password": String
        }
        Răspuns:
        🍪 Întoarce cookie de sesiune.

        Erori tratate:
        👎 Credențialele nu se potrivesc.
        👎 Reautentificare când deja logat.
*/
    // int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag)

    char * host_ip = "63.32.125.183";
    int portno = 8081;
    sockfd = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);



    char stdincmd[1000];
    while (1) {
        message = (char*)calloc(1000, sizeof(char));
        response = (char*)calloc(1000, sizeof(char));
        buffer buff = buffer_init();
        memset(stdincmd, 0, sizeof(stdincmd));
        fgets(stdincmd, sizeof(stdincmd), stdin);
        buffer_add(&buff, stdincmd, sizeof(stdincmd));
        // printf("%s\n", buff.data);

        /*
        Comenzi
            1. Login_admin (2.5p)
            Oferă prompt pentru username și password.


            login_admin

            username=something
            password=something

            Input
            login_admin
            username=something
            password=something
            Output
            // Username: marian.panait1206 , Password: 766d189849e0

            SUCCESS: Admin autentificat cu succes
        */
        if (strcmp(buff.data, "exit\n") == 0) {
            buffer_destroy(&buff);
            break;
        }
        if (strcmp(buff.data, "login_admin\n") == 0) {
            printf("username=");
            fflush(stdout);
            char username[50];
            fgets(username, sizeof(username), stdin);
            replace_char(username, '\n', '\0');


            printf("password=");
            fflush(stdout);
            char password[50];
            fgets(password, sizeof(password), stdin);
            replace_char(password, '\n', '\0');


            compute_message(message, username);
            compute_message(message, password);
//SUCCESS: Admin autentificat cu succes
            login_admin(sockfd, username, password);

            char *msgrecv = receive_from_server(sockfd);

            printf("%s", msgrecv);
            fflush(stdout);

        }













        free(response);
        free(message);
        buffer_destroy(&buff);

    }

    close_connection(sockfd);
    return 0;
}
