
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

void deallocate_memory1(char *response, char *message,
                        char *host, char *url, char *ip, char *code)
{
    // Free
    free(response);
    free(message);

    // Deallocate the memory
    delete[] host;
    delete[] url;
    delete[] ip;
    delete[] code;
}

void deallocate_memory2(char *response, char *message,
                        char *host, char *url, char *ip, char *code, char *id)
{
    // Free
    free(response);
    free(message);

    // Deallocate the memory
    delete[] host;
    delete[] url;
    delete[] ip;
    delete[] code;
    delete[] id;
}

void deallocate_memory3(char *response, char *message,
                        char *host, char *url, char *content_type, char *ip, char *code, char *title,
                        char *author, char *genre, char *publisher, char *page_count)
{
    // Free
    free(response);
    free(message);

    // Deallocate the memory
    delete[] host;
    delete[] url;
    delete[] content_type;
    delete[] ip;
    delete[] code;
    delete[] title;
    delete[] author;
    delete[] genre;
    delete[] publisher;
    delete[] page_count;
}

void extract_code(char *response, char *code)
{
    // Extract HTTP code from response
    strncpy(code, response + 9, 3);
    code[3] = '\0';
    cout << "Return code: " << code << endl;
}

void extract_cookie(char *response, char *cookie)
{
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

bool isNumber(string id)
{
    for (int i = 0; i < id.size(); i++)
    {
        if (!isdigit(id[i]))
        {
            return false;
        }
    }
    return true;
}

void id_prompt(char *id)
{
    // Read book id from user
    string id_str;
    bool is_number = false;

    while (!is_number)
    {
        cout << "Book ID: ";
        cin >> id_str;

        // Check if the id is negative
        if (id_str[0] == '-')
        {
            cout << "Book ID must be positive!" << endl;
            continue;
        }

        // Check if the id starts with 0
        if (id_str[0] == '0')
        {
            cout << "Book ID must not start with 0!" << endl;
            continue;
        }

        is_number = isNumber(id_str);
        if (!is_number)
        {
            cout << "Book ID must be a number!" << endl;
        }
    }

    // Copy the strings to the allocated memory
    std::strcpy(id, id_str.c_str());
}

void book_prompt(char *title, char *author, char *genre, char *page_count, char *publisher)
{
    // Read book fields from user
    cout << "Title: ";
    fgets(title, MAX_TITLE_LEN, stdin);
    fgets(title, MAX_TITLE_LEN, stdin);
    title[strlen(title) - 1] = '\0';
    puts(title);

    cout << "Author: ";
    fgets(author, MAX_AUTHOR_LEN, stdin);
    author[strlen(author) - 1] = '\0';
    puts(author);

    cout << "Genre: ";
    fgets(genre, MAX_GENRE_LEN, stdin);
    genre[strlen(genre) - 1] = '\0';
    puts(genre);

    // Check if the input for page count is a number
    bool is_number = false;
    while (!is_number)
    {
        cout << "Page count: ";
        fgets(page_count, MAX_PAGE_COUNT_LEN, stdin);
        page_count[strlen(page_count) - 1] = '\0';
        puts(page_count);

        // Check if the input is negative
        if (page_count[0] == '-')
        {
            cout << "Page count must be positive!" << endl;
            continue;
        }

        // Check if input starts with 0
        if (page_count[0] == '0')
        {
            cout << "Page count must not start with 0!" << endl;
            continue;
        }

        // Check if the input is a number
        is_number = true;
        for (int i = 0; i < strlen(page_count); i++)
        {
            if (!isdigit(page_count[i]))
            {
                is_number = false;
                cout << "Page count must be a number!" << endl;
                break;
            }
        }
    }

    cout << "Publisher: ";
    fgets(publisher, MAX_PUBLISHER_LEN, stdin);
    publisher[strlen(publisher) - 1] = '\0';
    puts(publisher);
}

void json_object_to_string(json &content_json, char *content)
{
    // Print the json fields
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
