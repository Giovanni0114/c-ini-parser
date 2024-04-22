#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

#include "control.h"

#define MAX_BUFF_SIZE 1024

bool detectInputOnStdin(void) {
    int input;
    return (ioctl(0, I_NREAD, &input) == 0 && input > 0);
}

bool readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    char buffer[MAX_BUFF_SIZE];

    if (file == NULL) {
        printf("File error: %s\n", strerror(errno));
        return false;
    }

    while (fgets(buffer, MAX_BUFF_SIZE, file)) {
        if (strchr(buffer, '=') == NULL) {
            continue;
        }

        char *key = strtok(buffer, "=");
        char *value = strtok(NULL, "\n");

        printf("Key: %s, Value: %s\n", key, value);
    }
    fclose(file);

    return true;
}

int main(void) {
    if (detectInputOnStdin()) {
        char line[1024];
        while (fgets(line, sizeof(line), stdin)) {
            printf("Read line: %s", line);
        }
    }

    if (!init_api()){
        fprintf(stderr, "unable to start_api\n");
        return get_status();
    };

    for (int i = 0; i < 3; i++) {
        fprintf(stderr, "%d\n", get_status());
        sleep(1);
    }
    add_endpoint("/23", callback_echo_json);

    for (int i = 0; i < 300; i++) {
        fprintf(stderr, "%d\n", get_status());
        sleep(1);
    }

    return !readFile("test.ini");
}
