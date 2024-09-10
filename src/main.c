#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "config.h"
#include "control.h"
#include "parser.h"

int detectInputOnStdin(void) {
    int input;
    if (ioctl(STDIN_FILENO, FIONREAD, &input) != 0){
        printf("STDIN error: %s\n", strerror(errno));
        return 0;
    }
    return input > 0;
}

void print_usage(const char *program_name) {
    printf(
        "Usage: %s -c /path/to/ini/file\nAlternativly give it to input: \"%s </path/to/ini/file\" "
        "or \"cat /path/to/ini/file | %s\"\n",
        program_name, program_name, program_name);
}

int main(int argc, char *argv[]) {
    char path[1024];

    Config config;
    initConfig(&config);

    if (detectInputOnStdin()) {
        char line[1024];
        while (fgets(line, sizeof(line), stdin)) {
            printf("Read line: %s", line);
        }
    } else if (argc >= 3) {
        strncpy(path, argv[2], 1024);
    } else if (strcmp(path, "0") == 0) {
        strncpy(path, config.path, 1024);
    } else {
        print_usage(argv[0]);
        return 1;
    }

    Section **content = loadFile(path);
    printSection(*content[0]);

    // todo:
    // all the shit lol

    return 1;
}
