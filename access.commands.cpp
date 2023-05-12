
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

    account_prompt(keys, values);

    // Open connection
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    char *content_type = new char[MAX_TYPE_LEN];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/auth/register");
    strcpy(content_type, "application/json");

    // Open connection and send the request
    message = compute_post_request(host, url, content_type, keys, values, CREDETIALS_COUNT, NULL, 0, NULL, 0);

    // Create the ip
    char *ip = new char[MAX_IP_SIZE];
    strcpy(ip, "34.254.242.81");

    // Open connection
    int sockfd = open_connection(ip, PORT, AF_INET, SOCK_STREAM, 0);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code);

    // If response contains code 201, the user was created
    strstr(response, "201 Created") != NULL ? cout << "User created succesfully." << endl
                                            : cout << "User already exists." << endl;

    // Deallocate the memory
    deallocate_memory(response, message, keys, values, host, url, content_type, ip, code);
}

void handle_login(char *cookie)
{
    // Check if the user is already logged in
    if (strcmp(cookie, "") != 0)
    {
        cout << "You are already logged in." << endl;
        return;
    }

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

    account_prompt(keys, values);

    // Open connection
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    char *content_type = new char[MAX_TYPE_LEN];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/auth/login");
    strcpy(content_type, "application/json");

    // Open connection and send the request
    message = compute_post_request(host, url, content_type, keys, values, CREDETIALS_COUNT, NULL, 0, NULL, 0);

    // Create the ip
    char *ip = new char[MAX_IP_SIZE];
    strcpy(ip, "34.254.242.81");

    // Open connection
    int sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code);

    // If response contains code 200, the user was logged in
    char *res = strstr(response, "200 OK");
    res != NULL ? cout << "User logged in succesfully." << endl
                : cout << "Wrong username or password." << endl;

    if (res != NULL)
    {
        // Extract the cookie
        extract_cookie(response, cookie);
    }

    // Deallocate the memory
    deallocate_memory(response, message, keys, values, host, url, content_type, ip, code);
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

    // Create the ip
    char *ip = new char[MAX_IP_SIZE];
    strcpy(ip, "34.254.242.81");

    // Open connection
    int sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code);

    // If response contains code 200, the user entered the library
    char *res = strstr(response, "200 OK");
    res != NULL ? cout << "Entered the library successfully." << endl
                : cout << "You are not logged in." << endl;

    if (res)
    {
        // Extract the cookie
        json token_json;
        extract_token_book(response, token_json);

        // Copy the token
        strcpy(token, token_json["token"].get<std::string>().c_str());
    }

    // Deallocate the memory
    deallocate_memory1(response, message, host, url, ip, code);
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

    // Create the ip
    char *ip = new char[MAX_IP_SIZE];
    strcpy(ip, "34.254.242.81");

    // Open connection
    int sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    // Send the request to the server
    send_to_server(sockfd, message);

    // Get the response from the server
    response = receive_from_server(sockfd);

    // Extract HTTP code from response
    char *code = new char[4];
    extract_code(response, code);

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
    deallocate_memory1(response, message, host, url, ip, code);

    // Invalidate token and cookie
    strcpy(token, "");
    strcpy(cookie, "");
}
