
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

void id_prompt(char *id)
{
    // Read username and pasword from user
    string id_str;
    cout << "Book ID: ";
    cin >> id_str;

    // Copy the strings to the allocated memory
    std::strcpy(id, id_str.c_str());
}

void book_prompt(char *title, char *author, char *genre, char *page_count, char *publisher)
{
    // TO DO make possible to get space in title names
    // Read username and pasword from user
    string title_str;
    string author_str;
    string genre_str;
    string publisher_str;
    string page_count_str;
    cout << "Title: ";
    cin >> title_str;
    cout << "Author: ";
    cin >> author_str;
    cout << "Genre: ";
    cin >> genre_str;

    bool is_number = false;
    while (!is_number)
    {
        cout << "Page count: ";
        cin >> page_count_str;

        // Check if input starts with 0
        if (page_count_str[0] == '0')
        {
            cout << "Page count must not start with 0!" << endl;
            continue;
        }

        // Check if the input is a number
        is_number = true;
        for (int i = 0; i < page_count_str.size(); i++)
        {
            if (!isdigit(page_count_str[i]))
            {
                is_number = false;
                cout << "Page count must be a number!" << endl;
                break;
            }
        }
    }

    cout << "Publisher: ";
    cin >> publisher_str;

    // Copy the strings to the allocated memory
    std::strcpy(title, title_str.c_str());
    std::strcpy(author, author_str.c_str());
    std::strcpy(genre, genre_str.c_str());
    std::strcpy(publisher, publisher_str.c_str());
    std::strcpy(page_count, page_count_str.c_str());
}

void json_list_to_string(json &content_json, char *content)
{
    // Print the json fields
    for (int i = 0; i < content_json.size(); i++)
    {
        strcat(content, "Title: ");
        strcat(content, content_json[i]["title"].get<string>().c_str());
        strcat(content, "\n");
        strcat(content, "ID: ");
        strcat(content, to_string(content_json[i]["id"].get<int>()).c_str());
        strcat(content, "\n");
        strcat(content, "\n");
    }
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
