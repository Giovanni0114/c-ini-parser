#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "control.h"
#include "parser.h"
#include "types/app_config.h"

int detectInputOnStdin(void) {
    int input;
    if (ioctl(STDIN_FILENO, FIONREAD, &input) != 0) {
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
    char *path = (char *)malloc(sizeof(char *));
    AppConfig *config = createDefaultAppConfig();

    if (detectInputOnStdin()) {
        fprintf(stderr, "Reading from stdin\n");
        strncpy(path, "/dev/stdin", sizeof("/dev/stdin"));
    } else if (argc >= 3 && strncmp(argv[1], "-c", 2) == 0) {
        strncpy(path, argv[2], strlen(argv[2]));
    } else if (path == NULL) {
        strncpy(path, config->path, strlen(config->path));
    } else {
        print_usage(argv[0]);
        return 1;
    }

    Section **content = loadFile(path);
    printSection(content[0]);

    // todo:
    // all the shit lol

    return 1;
}
