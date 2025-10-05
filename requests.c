#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
char *compute_get_request(char *host, char *url, char *query_params,
                        char **cookies, int cookies_count)
                        {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    
    sprintf(line, "Connection: keep-alive");
    compute_message(message, line);

    memset(line, 0, LINELEN);

    if (cookies[0][0] != '\0') {
        sprintf(line, "%s", cookies[0]);
        compute_message(message, line);
    }
    // Authorization: Bearer eijjkwuqioueu9182712093801293
    if (cookies[1][0] != '\0') {
        sprintf(line, "Authorization: Bearer %s", cookies[1]);
        compute_message(message, line);
    }

    compute_message(message, "");

    free(line);
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
    char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(BUFLEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    strcat(body_data_buffer, body_data);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);

    sprintf(line, "Connection: keep-alive");
    compute_message(message, line);

    if (cookies[0][0] != '\0') {
        sprintf(line, "%s", cookies[0]);
        compute_message(message, line);
    }
    // Authorization: Bearer eijjkwuqioueu9182712093801293
    if (cookies[1][0] != '\0') {
        sprintf(line, "Authorization: Bearer %s", cookies[1]);
        compute_message(message, line);
    }

    compute_message(message, "");

    compute_message(message, body_data_buffer);

    free(line);
    free(body_data_buffer);
    return message;
}

char *compute_delete_request(char *host, char *url,
                          char **cookies, int cookies_count) 
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Connection: keep-alive");
    compute_message(message, line);

    if (cookies[0][0] != '\0') {
        sprintf(line, "%s", cookies[0]);
        compute_message(message, line);
    }
    // Authorization: Bearer eijjkwuqioueu9182712093801293
    if (cookies[1][0] != '\0') {
        sprintf(line, "Authorization: Bearer %s", cookies[1]);
        compute_message(message, line);
    }


    compute_message(message, "");

    free(line);
    return message;
}
char * compute_put_request(char *host, char *url, char **cookies,
                            int cookies_count, char *content) 
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    sprintf(line, "PUT %s HTTP/1.1", url);
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Connection: keep-alive");
    compute_message(message, line);

    int len = strlen(content);

    sprintf(line, "Content-Type: application/json");
    compute_message(message, line);

    sprintf(line, "Content-Length: %d", len);
    compute_message(message, line);

    if (cookies[0][0] != '\0') {
        sprintf(line, "%s", cookies[0]);
        compute_message(message, line);
    }
    // Authorization: Bearer eijjkwuqioueu9182712093801293
    if (cookies[1][0] != '\0') {
        sprintf(line, "Authorization: Bearer %s", cookies[1]);
        compute_message(message, line);
    }

    compute_message(message, "");

    compute_message(message, content);

    free(line);

    return message;

}