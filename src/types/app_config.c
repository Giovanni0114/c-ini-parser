#include "types/app_config.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

#include "parser.h"

AppConfig *createEmptyAppConfig(void){
    return (AppConfig *)malloc(sizeof(AppConfig));
}

AppConfig *createDefaultAppConfig(void) {
    AppConfig *appConfig = createEmptyAppConfig();

    appConfig->port = 3000;
    appConfig->ensureExist = NULL;
    appConfig->path = "";

    return appConfig;
}

bool getIntFromLine(Line *line, int *ref) {
    char *end;
    int number;

    errno = 0;
    fprintf(stderr, "%s\n", line->value);
    number = strtol(line->value, &end, 10);

    if (end == line->value) {
        fprintf(stderr, "No digits were found\n");
        return false;
    }

    *ref = number;
    return true;
}

void splitAndStore(char *input, AppConfig *config) {
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

void initConfig(AppConfig *config) {
    config = createDefaultAppConfig();

    Section **configStruct = loadFile(CONFIG_FILE_PATH);

    if (configStruct[0] == NULL) {
        fprintf(stderr, "unable to load config\n");
        return;
    }

    // General
    Line *portLine = getLineByName(configStruct[0], "Port");
    Line *ensureExist = getLineByName(configStruct[0], "EnsureExist");

    if (portLine == NULL || !getIntFromLine(portLine, &config->port)) {
        fprintf(stderr, "unable to find Port in config, using default 3000\n");
        config->port = 3000;
    } 

    if (ensureExist == NULL) {
        fprintf(stderr, "unable to find EnsureExist in config, skipping\n");
        return;
    }

    splitAndStore(ensureExist->value, config);
}
