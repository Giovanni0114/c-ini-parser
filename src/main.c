#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

#include "config.h"
#include "control.h"
#include "parser.h"

int detectInputOnStdin(void) {
    int input;
    return (ioctl(0, I_NREAD, &input) == 0 && input > 0);
}

int main(void) {
    if (detectInputOnStdin()) {
        char line[1024];
        while (fgets(line, sizeof(line), stdin)) {
            printf("Read line: %s", line);
        }
    }

    Config *config = NULL;
    initConfig(config, "config.ini");

    char *next = NULL;
    Section *read = parseFile("test.ini", NULL, &next);
    printf("-> next: %s\n", next);
    if (read != NULL) {
        printSection(*read);
        free(read);
    }

    while (next != NULL) {
        char *target = strdup(next);
        next = NULL;
        read = parseFile("test.ini", target, &next);
        if (read != NULL) {
            printSection(*read);
            free(read);
        }
        free(target);
    }

    return 1;
}
