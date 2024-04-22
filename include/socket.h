#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int server_fd = -1;

#define SOCKET_PATH "/tmp/c-ini-parser-ap.socket"
#define SOCKET_PASSWORD "replace_me_for_the_fuck_sake"

int init_socket_server(void);
int is_server_initialised(void);
int revcv_from_server(void);
