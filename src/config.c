#include "config.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

#include "parser.h"

void assignDefault(Config *config) {
    config->port = -1;
    config->vaccumMode = V_MODE_NONE;
    config->vaccumModePeriod = V_PERIOD_NONE;
    config->ensureExist = NULL;
}

// TODO: requires changing logic
int getIntFromLine(IniLine *line) {
    char *end;
    int number;

    errno = 0;
    fprintf(stderr, "%s\n", line->value);
    number = strtol(line->value, &end, 10);

    if (end == line->value) {
        fprintf(stderr, "No digits were found\n");
        return -1;
    }
    return number;
}

void splitAndStore(char *input, Config *config) {
    char **placeholderForEnsured;
    int index = 0;
    const char delimiter[] = ",";

    {
        char *inputCopy = strdup(input);
        char *token = strtok(inputCopy, delimiter);
        while (token) {
            index++;
            token = strtok(NULL, delimiter);
        }
        free(inputCopy);
        free(token);
    }

    placeholderForEnsured = (char **)malloc((index + 1) * sizeof(char *));
    placeholderForEnsured[index] = NULL;

    {
        index = 0;
        char *inputCopy = strdup(input);
        char *token = strtok(inputCopy, delimiter);
        while (token) {
            placeholderForEnsured[index++] = strdup(token);
            token = strtok(NULL, delimiter);
        }
        free(inputCopy);
        free(token);
    }

    config->ensureExist = placeholderForEnsured;
}

void initConfig(Config *config) {
    config = malloc(sizeof(Config));
    if (config == NULL) {
        assert(0 && "BUY MORE RAM");
        return;
    }

    assignDefault(config);

    Section **configStruct = loadFile(CONFIG_FILE_PATH);

    if (configStruct[0] == NULL) {
        fprintf(stderr, "unable to load config\n");
        return;
    }

    IniLine *portLine = getLineByName(configStruct[0], "Port");
    IniLine *ensureExist = getLineByName(configStruct[0], "EnsureExist");

    if (portLine == NULL || getIntFromLine(portLine) == -1) {
        fprintf(stderr, "unable to find Port in config, using default 3000\n");
        config->port = 3000;
    } else {
        config->port = getIntFromLine(portLine);
    }

    if (ensureExist == NULL) {
        fprintf(stderr, "unable to find EnsureExist in config, skipping\n");
        return;
    } else {
        splitAndStore(ensureExist->value, config);
    }
}
