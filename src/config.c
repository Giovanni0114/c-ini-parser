#include "config.h"

#include "parser.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

void initConfig(Config *config, const char *filename) {
    config = malloc(sizeof(Config));
    if (config == NULL) {
        assert(0 && "BUY MORE RAM");
        return;
    }

    config->port = -1;
    config->vaccumMode = V_MODE_NONE;
    config->vaccumModePeriod = V_PERIOD_NONE;
    config->ensureExist = NULL;

    Section *configStruct = parseFile(filename, NULL, NULL);
    printSection(*configStruct);
}
