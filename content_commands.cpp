
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

void get_book_helper(char *cookie, char *token, char *id)
{
    // Check if the user is logged in andd has access
    if (!check_access(cookie, token))
        return;

    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Create the url, host and content type
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, id);

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
        cout << "Got the solicited book." << endl;
    }

    char *res = strstr(response, "403");
    if (res != NULL)
    {
        cout << "You are not logged in or you dont have access to the library." << endl;
    }

    res = strstr(response, "404");
    if (res != NULL)
    {
        cout << "No book with the given ID." << endl;
    }

    char *content = new char[MAX_CONTENT_LEN];
    if (res1 != NULL)
    {
        // Extract the content
        json content_json;
        extract_token_book(response, content_json);

        // Copy the content
        json_object_to_string(content_json, content);
        puts(content);
    }

    // Deallocate the memory
    deallocate_memory2(response, message, host, url, ip, code, id);
}

void handle_get_books(char *cookie, char *token)
{
    if (!check_access(cookie, token))
        return;

    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Open connection
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/library/books");

    char *cookies[MAX_COOKIE_COUNT] = {cookie};
    char *tokens[MAX_TOKEN_COUNT] = {token};

    // Open connection and send the request
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
    char *res = strstr(response, "200 OK");
    res != NULL ? cout << "Got the list of books." << endl
                : cout << "You are not logged in or you dont have access to the library." << endl;

    if (res)
    {
        // Extract the content
        json content_json = json::array();
        extract_list(response, content_json);

        // Get details for each book
        for (int i = 0; i < content_json.size(); i++)
        {
            // Allocate memory for the id
            char *id = new char[MAX_ID_LEN];

            // Get the id of the book
            strcpy(id, to_string(content_json[i]["id"].get<int>()).c_str());

            // Get the book details
            get_book_helper(cookie, token, id);
        }
    }

    // Deallocate the memory
    deallocate_memory1(response, message, host, url, ip, code);
}

void handle_get_book(char *cookie, char *token)
{
    // Ask for the id
    char *id = new char[MAX_ID_LEN];
    id_prompt(id);

    get_book_helper(cookie, token, id);
}

void handle_add_book(char *cookie, char *token)
{
    if (!check_access(cookie, token))
        return;

    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Create the url, host and content type
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    char *content_type = new char[MAX_TYPE_LEN];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/library/books");
    strcpy(content_type, "application/json");

    // Create field keys
    char title_k[MAX_TITLE_LEN] = "title";
    char author_k[MAX_AUTHOR_LEN] = "author";
    char genre_k[MAX_GENRE_LEN] = "genre";
    char page_count_K[MAX_PAGE_COUNT_LEN] = "page_count";
    char publisher_k[MAX_PUBLISHER_LEN] = "publisher";

    // Create field values
    char *title = new char[MAX_TITLE_LEN];
    char *author = new char[MAX_AUTHOR_LEN];
    char *genre = new char[MAX_GENRE_LEN];
    char *page_count = new char[MAX_PAGE_COUNT_LEN];
    char *publisher = new char[MAX_PUBLISHER_LEN];

    // Ask for the fields
    book_prompt(title, author, genre, page_count, publisher);

    // Create the keys
    char *keys[MAX_KEYS_COUNT] = {title_k, author_k, genre_k, page_count_K, publisher_k};
    char *values[MAX_VALUES_COUNT] = {title, author, genre, page_count, publisher};

    // Create cookies and tokens
    char *cookies[MAX_COOKIE_COUNT] = {cookie};
    char *tokens[MAX_TOKEN_COUNT] = {token};

    // Create the request
    message = compute_post_request(host, url, content_type, keys, values, BOOK_FIELDS_COUNT, cookies, 1, tokens, 1);

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
        cout << "Added the book." << endl;
    }

    char *res = strstr(response, "403");
    if (res != NULL)
    {
        cout << "You are not logged in or you dont have access to the library." << endl;
    }

    res = strstr(response, "404");
    if (res != NULL)
    {
        cout << "No book with the given ID." << endl;
    }

    // Deallocate the memory
    deallocate_memory3(response, message, host, url, content_type, ip, code, title, author, genre, publisher, page_count);
}

void handle_delete_book(char *cookie, char *token)
{
    // Ask for the id
    char *id = new char[MAX_ID_LEN];
    id_prompt(id);

    // Check if the user is logged in andd has access
    if (!check_access(cookie, token))
        return;

    // Allocate memory for the array elements
    char *message = (char *)malloc(BUFLEN * sizeof(char));
    char *response = (char *)malloc(MAX_RESPONE_LEN * sizeof(char));

    // Create the url, host and content type
    char *host = new char[MAX_HOST_LEN];
    char *url = new char[URL_SIZE];
    strcpy(host, "34.254.242.81:8080");
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, id);

    // Create cookies and tokens
    char *cookies[MAX_COOKIE_COUNT] = {cookie};
    char *tokens[MAX_TOKEN_COUNT] = {token};

    // Create the request
    message = compute_delete_request(host, url, cookies, 1, tokens, 1);

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
        cout << "Deleted the book successfully." << endl;
    }

    char *res = strstr(response, "403");
    if (res != NULL)
    {
        cout << "You are not logged in or you dont have access to the library." << endl;
    }

    res = strstr(response, "404");
    if (res != NULL)
    {
        cout << "No book with the given ID." << endl;
    }

    // Deallocate the memory
    deallocate_memory2(response, message, host, url, ip, code, id);
}
