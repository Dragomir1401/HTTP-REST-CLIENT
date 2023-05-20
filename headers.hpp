#ifndef _HEADER_H_
#define _HEADER_H_

#define BUFLEN 4096
#define LINELEN 1000
#define MAX_KEY_LEN 100
#define MAX_VALUE_LEN 100
#define MAX_RESPONE_LEN 10000
#define CREDETIALS_COUNT 2
#define MAX_IP_SIZE 15
#define URL_SIZE 50
#define MAX_TYPE_LEN 50
#define PORT 8080
#define MAX_HOST_LEN 30
#define MAX_COOKIE_LEN 1000
#define MAX_COOKIE_COUNT 10
#define MAX_CONTENT_LEN 50000
#define MAX_TOKEN_LEN 1000
#define MAX_TOKEN_COUNT 10
#define MAX_ID_LEN 10
#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100
#define MAX_GENRE_LEN 100
#define MAX_PUBLISHER_LEN 100
#define MAX_PAGE_COUNT_LEN 100
#define MAX_KEYS_COUNT 50
#define MAX_VALUES_COUNT 50
#define BOOK_FIELDS_COUNT 5
#define MAX_COMMAND_LEN 100
#include "json.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using json = nlohmann::json;

void handle_register();
void compute_message(char *message, const char *line);
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);
char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count, char **tokens, int tokens_count);
char *compute_post_request(char *host, char *url, char *content_type, char **keys, char **values,
                           int body_data_fields_count, char **cookies, int cookies_count, char **tokens, int tokens_count);
void send_to_server(int sockfd, char *message);
char *receive_from_server(int sockfd);
void close_connection(int sockfd);
void handle_login(char *cookie);
void handle_enter_library(char *cookie, char *token);
void handle_get_books(char *cookie, char *token);
void deallocate_memory(char *response, char *message, char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT],
                       char *host, char *url, char *content_type, char *code);
int account_prompt(char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT]);
void deallocate_memory1(char *response, char *message,
                        char *host, char *url, char *code);
void extract_code(char *response, char *code, int print_flag);
void extract_cookie(char *response, char *cookie);
int extract_list(char *response, json &content_json);
void extract_token_book(char *response, json &token_json);
void handle_get_book(char *cookie, char *token);
int id_prompt(char *id);
void deallocate_memory2(char *response, char *message,
                        char *host, char *url, char *code, char *id);
void json_object_to_string(json &content_json, char *content, char *id);
int book_prompt(char *title, char *author, char *genre, char *page_count, char *publisher);
void deallocate_memory3(char *response, char *message,
                        char *host, char *url, char *content_type, char *code, char *title,
                        char *author, char *genre, char *publisher, char *page_count);
void handle_add_book(char *cookie, char *token);
char *compute_delete_request(char *host, char *url, char **cookies, int cookies_count,
                             char **tokens, int tokens_count);
void handle_delete_book(char *cookie, char *token);
void handle_logout(char *cookie, char *token);
bool check_access(char *cookie, char *token);

#endif /* _HEADER_H_ */
