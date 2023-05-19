
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "headers.hpp"
#include "json.hpp"
using json = nlohmann::json;

using namespace std;
extern pthread_mutex_t mtx;
extern int sockfd;

void handle_register()
{
    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Prompt the user for username and password
    char *keys[CREDETIALS_COUNT];
    char *values[CREDETIALS_COUNT];

    // Allocate memory for the array elements
    keys[0] = new char[MAX_KEY_LEN];
    keys[1] = new char[MAX_KEY_LEN];
    values[0] = new char[MAX_VALUE_LEN];
    values[1] = new char[MAX_VALUE_LEN];

    int rc = account_prompt(keys, values);
    if (rc == -1)
    {
        return;
    }

    // Open connection
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    char *content_type = new char[MAX_TYPE_LEN];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/auth/register");
    strcpy(content_type, "application/json");

    // Open connection and send the request
    message = compute_post_request(host, url, content_type, keys, values, CREDETIALS_COUNT, NULL, 0, NULL, 0);

    pthread_mutex_lock(&mtx);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    pthread_mutex_unlock(&mtx);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code, 1);

    // If response contains code 201, the user was created
    strstr(response, "201 Created") != NULL ? cout << "User created succesfully." << endl
                                            : cout << "User already exists." << endl;

    // Deallocate the memory
    deallocate_memory(response, message, keys, values, host, url, content_type, code);
}

void handle_login(char *cookie)
{
    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Prompt the user for username and password
    char *keys[CREDETIALS_COUNT];
    char *values[CREDETIALS_COUNT];

    // Allocate memory for the array elements
    keys[0] = new char[MAX_KEY_LEN];
    keys[1] = new char[MAX_KEY_LEN];
    values[0] = new char[MAX_VALUE_LEN];
    values[1] = new char[MAX_VALUE_LEN];

    int rc = account_prompt(keys, values);
    if (rc == -1)
    {
        return;
    }

    // Open connection
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    char *content_type = new char[MAX_TYPE_LEN];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/auth/login");
    strcpy(content_type, "application/json");

    // Open connection and send the request
    message = compute_post_request(host, url, content_type, keys, values, CREDETIALS_COUNT, NULL, 0, NULL, 0);

    pthread_mutex_lock(&mtx);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    pthread_mutex_unlock(&mtx);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code, 1);

    // If cookie is not empty
    if (strcmp(cookie, ""))
    {
        // Update the cookie
        extract_cookie(response, cookie);

        cout << "User already logged in. Logout first if you want to switch acccounts." << endl;

        deallocate_memory(response, message, keys, values, host, url, content_type, code);
        return;
    }

    // If response contains code 200, the user was logged in succesfully
    char *res = strstr(response, "200 OK");
    if (res != NULL)
    {
        cout << "User logged in succesfully." << endl;
    }
    else
    {
        cout << "Wrong username or password." << endl;
    }

    if (res != NULL)
    {
        // Extract or update the cookie
        extract_cookie(response, cookie);
    }

    // Deallocate the memory
    deallocate_memory(response, message, keys, values, host, url, content_type, code);
}

void handle_enter_library(char *cookie, char *token)
{
    if (!strcmp(cookie, ""))
    {
        cout << "You must be logged in to access the library." << endl;
        return;
    }

    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Open connection
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/library/access");

    char *cookies[MAX_COOKIE_COUNT] = {cookie};
    // Open connection and send the request
    message = compute_get_request(host, url, NULL, cookies, 1, NULL, 0);

    pthread_mutex_lock(&mtx);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    pthread_mutex_unlock(&mtx);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code, 1);

    // If response contains code 200, the user entered the library
    char *res = strstr(response, "200 OK");
    if (res == NULL)
        cout << "You are not logged in." << endl;

    if (res)
    {
        // Extract the cookie
        json token_json;
        extract_token_book(response, token_json);

        // Copy the token
        strcpy(token, token_json["token"].get<std::string>().c_str());
    }

    // Deallocate the memory
    deallocate_memory1(response, message, host, url, code);
}

void handle_logout(char *cookie, char *token)
{
    if (!strcmp(cookie, ""))
    {
        cout << "You must be logged in to be able to log out" << endl;
        return;
    }

    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Create the url, host and content type
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/auth/logout");

    // Create cookies and tokens
    char *cookies[MAX_COOKIE_COUNT] = {cookie};
    char *tokens[MAX_TOKEN_COUNT] = {token};

    // Create the request
    message = compute_get_request(host, url, NULL, cookies, 1, tokens, 1);

    pthread_mutex_lock(&mtx);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    pthread_mutex_unlock(&mtx);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code, 1);

    // If response contains code 200, the user entered the library
    char *res1 = strstr(response, "200 OK");
    if (res1 != NULL)
    {
        cout << "Logout successful." << endl;
    }

    char *res = strstr(response, "400");
    if (res != NULL)
    {
        cout << "You are not logged in or you dont have access to the library." << endl;
    }
    else if (res1 == NULL && res == NULL)
    {
        cout << "Other errors occured." << endl;
    }

    // Deallocate the memory
    deallocate_memory1(response, message, host, url, code);

    // Invalidate token and cookie
    strcpy(token, "");
    strcpy(cookie, "");
}
