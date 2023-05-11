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
#define MAX_PAGE_COUNT_LEN 10
#include "json.hpp"
using json = nlohmann::json;

void handle_register();
void compute_message(char *message, const char *line);
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);
char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count, char **tokens, int tokens_count);
char *compute_post_request(char *host, char *url, char *content_type, char **keys, char **values,
                           int body_data_fields_count, char **cookies, int cookies_count);
void send_to_server(int sockfd, char *message);
char *receive_from_server(int sockfd);
void close_connection(int sockfd);
void handle_login(char *cookie);
void handle_enter_library(char *cookie, char *token);
void handle_get_books(char *cookie, char *token);
void deallocate_memory(char *response, char *message, char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT],
                       char *host, char *url, char *content_type, char *ip, char *code);
void account_prompt(char *keys[CREDETIALS_COUNT], char *values[CREDETIALS_COUNT]);

void deallocate_memory1(char *response, char *message,
                        char *host, char *url, char *ip, char *code);
void extract_code(char *response, char *code);
void extract_cookie(char *response, char *cookie);
void extract_list(char *response, json &content_json);
void extract_token_book(char *response, json &token_json);
void handle_get_book(char *cookie, char *token);
void id_prompt(char *id);
void deallocate_memory2(char *response, char *message,
                        char *host, char *url, char *content_type, char *ip, char *code, char *id);
void json_list_to_string(json &content_json, char *content);
void json_object_to_string(json &content_json, char *content);
void book_prompt(char *title, char *author, char *genre, char *publisher, char *page_count);

void deallocate_memory3(char *response, char *message,
                        char *host, char *url, char *content_type, char *ip, char *code, char *title,
                        char *author, char *genre, char *publisher, char *page_count);

#endif /* _HEADER_H_ */
