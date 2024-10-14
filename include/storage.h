#include "types/config.h"
#include "parser.h"

#define MAX_NUM_OF_SECTIONS 1024

static Config globalConfiguration;

Config *getGlobalConfigInstance(void);
