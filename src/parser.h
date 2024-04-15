#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>
#include <uuid/uuid.h>

#include "config.h"

typedef enum { TYPE_UNDEFINED = 0, TYPE_VALUE, TYPE_SECTION, TYPE_COMMENT } IniLineType;

typedef struct IniLine {
    char name[1024];
    char value[1024];
    IniLineType type;
    struct IniLine *nextLineInSection;
} IniLine;

typedef struct {
    char displayName[1024];
    uuid_t uuid;
    IniLine *firstLineInSection;
} Section;

#define MAX_NUM_OF_SECTIONS 1024

static Section configuration[MAX_NUM_OF_SECTIONS];

// CREATE
Section *newSection(const char *name);

IniLine *addLineInSection(Section *section, IniLine *line);
IniLine *newLineInSection(Section *section, char *name, char *value);

void appendLineToSection(Section *section, IniLine *line);

// READ
Section *getSectionByName(const char *name);
Section *getSection(Section *section);

IniLine *getLineByIndex(Section *section, int index);
IniLine *getLineByName(Section *section, char *name);

// UPDATES
void updateSectionName(Section *section, const char *newDisplayName);
void updateLineValue(Section *section, IniLine *line, const char *newValue);

// DELETE
bool deleteSectionByName(const char *name);
bool deleteSection(Section *section);

bool deleteLineByName(Section *section, const char *name);
bool deleteLine(Section *section, IniLine *line);

// UTILS
bool isConfgirationValid(void);

inline bool isSectionEmpty(Section *section) {
    return section->firstLineInSection == NULL;
}

static inline bool isSectionEmptyByIndex(int index) {
    return configuration[index].firstLineInSection == NULL;
}

