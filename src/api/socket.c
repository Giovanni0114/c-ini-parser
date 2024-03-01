#include "socket.h"

int is_server_initialised(void) {
    return server_fd != -1;
}

int init_socket_server(void) {
    struct sockaddr_un addr;
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd < 0) {
        fprintf(stderr, "unable to create socket");
        exit(EXIT_FAILURE);
    }

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    unlink(SOCKET_PATH);  // Ensure the file does not already exist

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "unable to bind socket to file " SOCKET_PATH);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        fprintf(stderr, "unable to listen on server socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

// int revcv_from_server(void){
//     printf("waiting to recvfrom...\n");
//     bytes_rec = recvfrom(server_sock, buf, 256, 0, (struct sockaddr *) &peer_sock, &len);
//     if (bytes_rec == -1){
//         printf("RECVFROM ERROR = %d", sock_errno());
//         // TODO probably should not close
//         close(server_sock);
//         exit(EXIT_FAILURE);
//     }
//     else {
//        printf("DATA RECEIVED = %s\n", buf);
//     }
// }
//
/*
 *
 */
