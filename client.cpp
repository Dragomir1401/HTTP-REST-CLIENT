#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <pthread.h>
#include <unistd.h>
#include "headers.hpp"
using namespace std;
// Open connection

pthread_mutex_t mtx;
int sockfd;

void *dummy(void *arg)
{
    while (true)
    {
        sleep(4);
        pthread_mutex_lock(&mtx);

        // Allocate memory for the array elements
        char *message = (char *)malloc(BUFLEN * sizeof(char));
        char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

        // Open connection
        char *host = new char[MAX_HOST_LEN];
        char *url = new char[URL_SIZE];
        strcpy(host, "34.254.242.81:8080");
        strcpy(url, "/api/v1/dummy");

        // Open connection and send the request
        message = compute_get_request(host, url, NULL, NULL, 0, NULL, 0);

        // Send the request to the server
        send_to_server(sockfd, message);

        // Get the response from the server
        response = receive_from_server(sockfd);

        pthread_mutex_unlock(&mtx);
    }

    return nullptr;
}

int main(int argc, char *argv[])
{
    // Create the ip
    char *ip = new char[MAX_IP_SIZE];
    strcpy(ip, "34.254.242.81");

    // Create a thread that makes dummy request every 4 seconds
    pthread_t workerThread;

    // Create the mutex
    pthread_mutex_init(&mtx, nullptr);

    // Create the thread
    pthread_create(&workerThread, nullptr, dummy, nullptr);

    // Open connection
    sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    char *command = new char[MAX_COMMAND_LEN];
    char *cookie = new char[MAX_COOKIE_LEN];
    char *token = new char[MAX_TOKEN_LEN];
    strcpy(cookie, "");
    strcpy(token, "");
    bool running = true;

    while (running)
    {
        cout << "Enter command: ";
        fgets(command, MAX_AUTHOR_LEN, stdin);
        // Remove the \n from the end of the string
        command[strcspn(command, "\n")] = '\0';

        if (!strncmp(command, "exit", 4) && strlen(command) == 4)
        {
            cout << "Exiting..." << endl;
            running = false;
        }
        else if (!strncmp(command, "register", 8) && strlen(command) == 8)
        {
            handle_register();
        }
        else if (!strncmp(command, "login", 5) && strlen(command) == 5)
        {
            handle_login(cookie);
        }
        else if (!strncmp(command, "enter_library", 13) && strlen(command) == 13)
        {
            handle_enter_library(cookie, token);
        }
        else if (!strncmp(command, "get_books", 9) && strlen(command) == 9)
        {
            handle_get_books(cookie, token);
        }
        else if (!strncmp(command, "get_book", 8) && strlen(command) == 8)
        {
            handle_get_book(cookie, token);
        }
        else if (!strncmp(command, "add_book", 8) && strlen(command) == 8)
        {
            handle_add_book(cookie, token);
        }
        else if (!strncmp(command, "delete_book", 11) && strlen(command) == 11)
        {
            handle_delete_book(cookie, token);
        }
        else if (!strncmp(command, "logout", 6) && strlen(command) == 6)
        {
            handle_logout(cookie, token);
        }
        else
        {
            cout << "Invalid command!" << endl;
        }
    }

    // Cancel the thread
    pthread_cancel(workerThread);

    // Destroy the mutex
    pthread_mutex_destroy(&mtx);

    // Deallocate memory
    delete[] ip;
    return 0;
}
