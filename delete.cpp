#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;
#include "headers.hpp"
using namespace std;

char *compute_delete_request(char *host, char *url, char **cookies, int cookies_count,
                             char **tokens, int tokens_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));

    // Write the method name, URL and protocol type
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    // Add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Add necessary headers
    snprintf(line, LINELEN, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);

    // Add cookies
    if (cookies != NULL)
    {
        for (int i = 0; i < cookies_count; i++)
        {
            snprintf(line, LINELEN, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }

    // Add tokens
    if (tokens != NULL)
    {
        for (int i = 0; i < tokens_count; i++)
        {
            snprintf(line, LINELEN, "Authorization: Bearer %s", tokens[i]);
            compute_message(message, line);
        }
    }

    // Add new line at end of header
    compute_message(message, "");

    // Add the actual payload data
    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);

    free(line);
    free(body_data_buffer);
    return message;
}
