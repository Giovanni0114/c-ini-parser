#include "callbacks.h"
#include "control.h"
#include "socket.h"

void handle_command(char *buffer) {
    if (strcmp(buffer, "stop") == 0) {
        printf("Server stopping...\n");
        stop_api();
        exit(0);
    } else if (strcmp(buffer, "reload") == 0) {
        printf("Reloading server configuration...\n");
        restart_api();

    } else {
        fprintf(stderr, "Received unknown command.\n");
    }
}

void register_ini_section_on_api(char *name, Callback callback, void *data) {
    char endpoint[100];
    sprintf(endpoint, "/%s", name);
    add_endpoint(endpoint, callback);
}

void main_loop(void) {
    struct sockaddr_un address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    while (1) {
        if (is_error_occured()) {
            fprintf(stderr, "error occured, exiting app");
            exit(1);
        }

        sleep(1);
    }
}

int init_socket_client(void) {
    int client_socket;
    struct sockaddr_un address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    printf("Listening on socket %s...\n", SOCKET_PATH);

    while(1) {
        if (!is_server_initialised()){
            fprintf(stderr, "Unable to connect to uninitialised server");
            exit(EXIT_FAILURE);
        }
        printf("Waiting for commands...\n");
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (client_socket < 0) {
            fprintf(stderr, "Unable to create socket server");
            exit(EXIT_FAILURE);
        }

        long read_val = read(client_socket, buffer, 1024);
        if (read_val > 0) {
            printf("Received command: %s\n", buffer);
            if (strncmp(buffer, SOCKET_PASSWORD, strlen(SOCKET_PASSWORD)) == 0) {
                handle_command(buffer);
            } else {
                printf("Authentication failed.\n");
            }
        }
        close(client_socket);
        sleep(1);
    }

    return 0;
}

#ifdef API_TEST
int main(void) {
    init_api();
    add_endpoint("/helloworld", &hello_world_callback);
    register_ini_section_on_api("hw", &hello_world_callback, NULL);

    printf("TEST API started successfully");

    main_loop();
    return 0;
}
#endif /* ifdef API-TEST */
