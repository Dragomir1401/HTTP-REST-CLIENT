#ifndef _HEADER_H_
#define _HEADER_H_

#define BUFLEN 4096
#define LINELEN 1000

void handle_register();
void compute_message(char *message, const char *line);
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);
char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count);
char *compute_post_request(char *host, char *url, char *content_type, char **keys, char **values,
                           int body_data_fields_count, char **cookies, int cookies_count);
void send_to_server(int sockfd, char *message);
char *receive_from_server(int sockfd);
void handle_login();
#endif /* _HEADER_H_ */
