#include <stdbool.h>
#include <stddef.h>

#include "types/line.h"
#include "types/section.h"

#define MAX_BUFF_SIZE 1024

typedef enum LineType {
    TYPE_SECTION,
    TYPE_COMMENT,
    TYPE_VALUE,
    TYPE_UNDEFINED,
} LineType;

Section **loadFile(const char *filename);
Section *parseFile(const char *filename, char *target, char **next);

// UTILS
// bool isConfigurationValid(void);
// bool isSectionEmptyByIndex(int index);
