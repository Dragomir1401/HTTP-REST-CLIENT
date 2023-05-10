#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "headers.h"
using namespace std;

int main(int argc, char *argv[])
{
    string command;

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
            handle_login();
        }
        else if (!strncmp(command.c_str(), "enter_library", 13))
        {
            system(command.c_str());
        }
        else if (!strncmp(command.c_str(), "get_books", 9))
        {
            system(command.c_str());
        }
        else if (!strncmp(command.c_str(), "get_book", 8))
        {
            system(command.c_str());
        }
        else if (!strncmp(command.c_str(), "add_book", 8))
        {
            system(command.c_str());
        }
        else if (!strncmp(command.c_str(), "delete_book", 11))
        {
            system(command.c_str());
        }
        else if (!strncmp(command.c_str(), "logout", 6))
        {
            system(command.c_str());
        }
        else
        {
            cout << "Invalid command" << endl;
        }
    }

    return 0;
}