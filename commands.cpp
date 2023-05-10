
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

void handle_register()
{
    // Read username and pasword from user
    string username;
    string password;
    cout << "username: ";
    cin >> username;
    cout << "password: ";
    cin >> password;

    char *message;
    char *response;
    // Allocate memory for the array elements
    char *keys[2];
    char *values[2];
    keys[0] = new char[10];
    keys[1] = new char[10];
    values[0] = new char[username.length() + 1];
    values[1] = new char[password.length() + 1];

    // Copy the strings to the allocated memory
    std::strcpy(keys[0], "username");
    std::strcpy(keys[1], "password");
    std::strcpy(values[0], username.c_str());
    std::strcpy(values[1], password.c_str());

    // Open connection
    char *host = new char[15];
    char *ip = new char[15];
    char *url = new char[50];
    char *content_type = new char[50];
    strcpy(host, "34.254.242.81:8080");
    strcpy(ip, "34.254.242.81");
    strcpy(url, "/api/v1/tema/auth/register");
    strcpy(content_type, "application/json");

    int sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request(host, url, content_type, keys, values, 2, NULL, 0);
    puts(message);
    puts("\n\n");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    puts("\n\n");

    // Deallocate the memory
    delete[] keys[0];
    delete[] keys[1];
    delete[] values[0];
    delete[] values[1];
}

void handle_login()
{
}
