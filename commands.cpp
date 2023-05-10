
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
#include "headers.h"

using namespace std;

void account_prompt(char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT])
{
    // Read username and pasword from user
    string username;
    string password;
    cout << "username: ";
    cin >> username;
    cout << "password: ";
    cin >> password;

    // Copy the strings to the allocated memory
    std::strcpy(keys[0], "username");
    std::strcpy(keys[1], "password");
    std::strcpy(values[0], username.c_str());
    std::strcpy(values[1], password.c_str());
}

void deallocate_memory(char *response, char *message, char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT],
                       char *host, char *url, char *content_type, char *ip, char *code)
{
    // Free
    free(response);
    free(message);

    // Deallocate the memory
    delete[] keys[0];
    delete[] keys[1];
    delete[] values[0];
    delete[] values[1];
    delete[] host;
    delete[] url;
    delete[] content_type;
    delete[] ip;
    delete[] code;
}

void extract_code(char *response, char *code)
{
    // Extract HTTP code from response
    strncpy(code, response + 9, 3);
    code[3] = '\0';
    cout << "Return code: " << code << endl;
}

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
    message = compute_post_request(host, url, content_type, keys, values, CREDETIALS_COUNT, NULL, 0);

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

    // Close connection
    close_connection(sockfd);
}

void extract_cookie(char *response, char *cookie)
{
    // To do
}

void handle_login()
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
    strcpy(url, "/api/v1/tema/auth/login");
    strcpy(content_type, "application/json");

    // Open connection and send the request
    message = compute_post_request(host, url, content_type, keys, values, CREDETIALS_COUNT, NULL, 0);

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
        char *cookie = new char[MAX_COOKIE_LEN];
        extract_cookie(response, cookie);
    }

    // Deallocate the memory
    deallocate_memory(response, message, keys, values, host, url, content_type, ip, code);

    // Close connection
    close_connection(sockfd);
}
