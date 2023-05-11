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
#include "headers.h"
using namespace std;

char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count, char **tokens, int tokens_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Write the method name, URL, request params (if any) and protocol type
    query_params != NULL ? sprintf(line, "GET %s?%s HTTP/1.1", url, query_params) : sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    // Add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Add cookies, according to the protocol format
    if (cookies != NULL)
    {
        for (int i = 0; i < cookies_count; i++)
        {
            snprintf(line, LINELEN, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }

    // Add tokens, according to the protocol format
    if (tokens != NULL)
    {
        for (int i = 0; i < tokens_count; i++)
        {
            snprintf(line, LINELEN, "Authorization: Bearer %s", tokens[i]);
            compute_message(message, line);
        }
    }

    // Add final new line
    snprintf(line, LINELEN, "\n");
    compute_message(message, "");

    return message;
}
