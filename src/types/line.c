#include "types/line.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

Line* createEmptyLine(void) {
    return (Line*)malloc(sizeof(Line));
}

Line* createLine(const char* name, const char* value) {
    Line* line = createEmptyLine();

    if (!line) {
        return NULL;
    }

    strncpy(line->name, name, sizeof(line->name) - 1);
    line->name[sizeof(line->name) - 1] = '\0';

    strncpy(line->value, value, sizeof(line->value) - 1);
    line->value[sizeof(line->value) - 1] = '\0';

    return line;
}
