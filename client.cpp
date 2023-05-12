#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "headers.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    string command;
    char *cookie = new char[MAX_COOKIE_LEN];
    char *token = new char[MAX_TOKEN_LEN];
    strcpy(cookie, "");
    strcpy(token, "");
    bool running = true;

    while (running)
    {
        cout << "Enter command: ";
        cin >> command;
        if (!strncmp(command.c_str(), "exit", 4))
        {
            running = false;
        }
        else if (!strncmp(command.c_str(), "register", 8))
        {
            handle_register();
        }
        else if (!strncmp(command.c_str(), "login", 5))
        {
            handle_login(cookie);
        }
        else if (!strncmp(command.c_str(), "enter_library", 13))
        {
            handle_enter_library(cookie, token);
        }
        else if (!strncmp(command.c_str(), "get_books", 9))
        {
            handle_get_books(cookie, token);
        }
        else if (!strncmp(command.c_str(), "get_book", 8))
        {
            handle_get_book(cookie, token);
        }
        else if (!strncmp(command.c_str(), "add_book", 8))
        {
            handle_add_book(cookie, token);
        }
        else if (!strncmp(command.c_str(), "delete_book", 11))
        {
            handle_delete_book(cookie, token);
        }
        else if (!strncmp(command.c_str(), "logout", 6))
        {
            handle_logout(cookie, token);
        }
        else
        {
            cout << "Invalid command" << endl;
        }
    }

    return 0;
}
