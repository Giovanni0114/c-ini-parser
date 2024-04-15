#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

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

    return readFile("test.ini");
}
