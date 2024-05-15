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

int main(void) {
    if (detectInputOnStdin()) {
        char line[1024];
        while (fgets(line, sizeof(line), stdin)) {
            printf("Read line: %s", line);
        }
    }

    Config *config = NULL;
    initConfig(config);

    Section **content = loadFile("test.ini");
    printSection(*content[0]);

    return 1;
}
