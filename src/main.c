#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

#include "types.c"

#define MAX_BUFF_SIZE 1024

bool detectInputOnStdin() {
    int c;
    if (ioctl(0, I_NREAD, &c) == 0 && c > 0) return true;
    return false;
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
};

int main() {
    if (detectInputOnStdin()) {
        char line[1024];
        while (fgets(line, sizeof(line), stdin)) {
            printf("Read line: %s", line);
        }
    }

    return readFile("test.ini");
}
