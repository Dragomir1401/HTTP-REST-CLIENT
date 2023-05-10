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
// for convenience
using json = nlohmann::json;
#include "headers.h"
using namespace std;

char *compute_post_request(char *host, char *url, char *content_type, char **keys, char **values,
                           int body_data_fields_count, char **cookies, int cookies_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));

    // Write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    // Add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Add necessary headers (Content-Type and Content-Length are mandatory)
    // in order to write Content-Length you must first compute the message size
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    json j;

    for (int i = 0; i < body_data_fields_count; i++)
    {
        j[keys[i]] = values[i];
    }
    // Serialize the JSON
    string s = j.dump();
    strcpy(body_data_buffer, s.c_str());

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

    // Add new line at end of header
    compute_message(message, "");

    // Add the actual payload data
    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);

    free(line);
    return message;
}
