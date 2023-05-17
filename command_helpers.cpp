
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

int account_prompt(char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT])
{
    // Read username and pasword from user
    char *username = new char[MAX_VALUE_LEN];
    char *password = new char[MAX_VALUE_LEN];
    cout << "username: ";
    fgets(username, MAX_VALUE_LEN, stdin);
    cout << "password: ";
    fgets(password, MAX_VALUE_LEN, stdin);

    // Check if the username and password are valid
    if (username[strlen(username) - 1] == '\n')
    {
        username[strlen(username) - 1] = '\0';
    }
    if (password[strlen(password) - 1] == '\n')
    {
        password[strlen(password) - 1] = '\0';
    }

    // Check if the username and password are valid
    if (strlen(username) == 0 || strlen(password) == 0)
    {
        cout << "Username or password is empty." << endl;
        return -1;
    }

    // Check to see if they contain spaces
    if (strchr(username, ' ') != NULL || strchr(password, ' ') != NULL)
    {
        cout << "Username or password contains spaces." << endl;
        return -1;
    }

    // Copy the strings to the allocated memory
    std::strcpy(keys[0], "username");
    std::strcpy(keys[1], "password");
    std::strcpy(values[0], username);
    std::strcpy(values[1], password);

    return 1;
}

void deallocate_memory(char *response, char *message, char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT],
                       char *host, char *url, char *content_type, char *code)
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
    delete[] code;
}

void deallocate_memory1(char *response, char *message,
                        char *host, char *url, char *code)
{
    // Free
    free(response);
    free(message);

    // Deallocate the memory
    delete[] host;
    delete[] url;
    delete[] code;
}

void deallocate_memory2(char *response, char *message,
                        char *host, char *url, char *code, char *id)
{
    // Free
    free(response);
    free(message);

    // Deallocate the memory
    delete[] host;
    delete[] url;
    delete[] code;
    delete[] id;
}

void deallocate_memory3(char *response, char *message,
                        char *host, char *url, char *content_type, char *code, char *title,
                        char *author, char *genre, char *publisher, char *page_count)
{
    // Deallocate the memory
    delete[] response;
    delete[] message;
    delete[] host;
    delete[] url;
    delete[] content_type;
    delete[] code;
    delete[] title;
    delete[] author;
    delete[] genre;
    delete[] publisher;
    delete[] page_count;
}

void extract_code(char *response, char *code, int print_flag)
{
    // Extract HTTP code from response
    strncpy(code, response + 9, 3);
    code[3] = '\0';
    if (print_flag == 0)
    {
        return;
    }

    cout << "Return code: " << code << " ";

    // If code is 2xx then print ok
    if (code[0] == '2')
    {
        cout << "(OK)" << endl;
    }
    else if (code[0] == '3')
    {
        cout << "(REDIRECTING)" << endl;
    }
    else if (code[0] == '4')
    {
        cout << "(ERROR)" << endl;
    }
    else if (code[0] == '5')
    {
        cout << "(SERVER ERROR)" << endl;
    }
}

void extract_cookie(char *response, char *cookie)
{
    // Initialize cookie
    memset(cookie, 0, MAX_COOKIE_LEN);

    // Extract cookie from response
    char *set_cookie = strstr(response, "Set-Cookie:");

    char *token = strtok(set_cookie, " ;");
    token = strtok(NULL, " ;");

    strcpy(cookie, token);
}

void extract_token_book(char *response, json &token_json)
{
    // Extract content from response
    char *token = strtok(response, "{");
    token = strtok(NULL, "}");

    // Create the content
    char *content = (char *)malloc((strlen(token) + 2) * sizeof(char));
    strcpy(content, "{");
    strcat(content, token);
    strcat(content, "}");

    // Create the json object
    token_json = json::parse(content);
}

void extract_list(char *response, json &content_json)
{
    // Extract content from response
    char *token = strtok(response, "[");
    token = strtok(NULL, "]");

    // Create the content
    char *content = (char *)malloc((strlen(token) + 2) * sizeof(char));
    strcpy(content, "[");
    strcat(content, token);
    strcat(content, "]");

    // Create the json object
    content_json = json::parse(content);
}

bool isNumber(char *id)
{
    for (int i = 0; i < strlen(id); i++)
    {
        if (!isdigit(id[i]))
        {
            return false;
        }
    }
    return true;
}

int id_prompt(char *id)
{
    // Read book id from user
    cout << "Book ID: ";
    fgets(id, MAX_ID_LEN, stdin);
    id[strlen(id) - 1] = '\0';

    // Check if the id is negative
    if (id[0] == '-')
    {
        cout << "Book ID must be positive!" << endl;
        return -1;
    }

    // Check if the id starts with 0
    if (id[0] == '0')
    {
        cout << "Book ID must not start with 0!" << endl;
        return -1;
    }

    if (!isNumber(id))
    {
        cout << "Book ID must be a number and not contain spaces!" << endl;
        return -1;
    }

    return 1;
}

int book_prompt(char *title, char *author, char *genre, char *page_count, char *publisher)
{
    // Read book fields from user
    cout << "Title: ";
    fgets(title, MAX_TITLE_LEN, stdin);
    title[strlen(title) - 1] = '\0';

    // Check for empty title
    if (strlen(title) == 0)
    {
        cout << "Title must not be empty!" << endl;
        return -1;
    }

    cout << "Author: ";
    fgets(author, MAX_AUTHOR_LEN, stdin);
    author[strlen(author) - 1] = '\0';

    // Check for empty author
    if (strlen(author) == 0)
    {
        cout << "Author must not be empty!" << endl;
        return -1;
    }

    cout << "Genre: ";
    fgets(genre, MAX_GENRE_LEN, stdin);
    genre[strlen(genre) - 1] = '\0';

    // Check for empty genre
    if (strlen(genre) == 0)
    {
        cout << "Genre must not be empty!" << endl;
        return -1;
    }

    cout << "Publisher: ";
    fgets(publisher, MAX_PUBLISHER_LEN, stdin);
    publisher[strlen(publisher) - 1] = '\0';

    // Check for empty publisher
    if (strlen(publisher) == 0)
    {
        cout << "Publisher must not be empty!" << endl;
        return -1;
    }

    // Check if the input for page count is a number
    cout << "Page count: ";
    fgets(page_count, MAX_PAGE_COUNT_LEN, stdin);
    page_count[strlen(page_count) - 1] = '\0';
    puts(page_count);

    // Check for empty page count
    if (strlen(page_count) == 0)
    {
        cout << "Page count must not be empty!" << endl;
        return -1;
    }

    // Check if the input is negative
    if (page_count[0] == '-')
    {
        cout << "Page count must be positive!" << endl;
        return -1;
    }

    // Check if input starts with 0
    if (page_count[0] == '0')
    {
        cout << "Page count must not start with 0!" << endl;
        return -1;
    }

    // Check if the input is a number
    if (isNumber(page_count) == false)
    {
        cout << "Page count must be a number and not contain spaces!" << endl;
        return -1;
    }

    return 1;
}

void json_object_to_string(json &content_json, char *content, char *id)
{
    // Print the json fields
    strcat(content, "ID: ");
    strcat(content, id);
    strcat(content, "\n");
    strcat(content, "Title: ");
    strcat(content, content_json["title"].get<string>().c_str());
    strcat(content, "\n");
    strcat(content, "Author: ");
    strcat(content, content_json["author"].get<string>().c_str());
    strcat(content, "\n");
    strcat(content, "Genre: ");
    strcat(content, content_json["genre"].get<string>().c_str());
    strcat(content, "\n");
    strcat(content, "Publisher: ");
    strcat(content, content_json["publisher"].get<string>().c_str());
    strcat(content, "\n");
    strcat(content, "Page count: ");
    strcat(content, to_string(content_json["page_count"].get<int>()).c_str());
    strcat(content, "\n");
    strcat(content, "\n");
}

bool check_access(char *cookie, char *token)
{
    if (!strcmp(cookie, ""))
    {
        cout << "You must be logged in to access library contents." << endl;
        return false;
    }

    if (!strcmp(token, ""))
    {
        cout << "You must have entered the library to access the library contents. " << endl;
        return false;
    }

    return true;
}
