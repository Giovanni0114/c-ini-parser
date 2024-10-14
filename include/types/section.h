#pragma once

#include <stdbool.h>
#include <uuid/uuid.h>

#include "types/line.h"

typedef struct Section {
    char *name;
    uuid_t uuid;
    Line *firstLineInSection;
    struct Section *nextSectionInConfig;
} Section;

// creation
Section *createEmptySection(void);
Section *createSection(const char *name);

// get child
Line *getLineByName(Section *section, char *name);
Line *getLineAt(Section *section, int index);
Line *getLastLine(Section *section);

// add child
void appendLineToSection(Section *section, Line *line);
bool insertLinkedListObject(Section *section, Line *line, int idx);

// delete child 
bool deleteLineAt(Line *Line, int idx);
void deleteAllLines(Section *section);

// update
void updateSectionName(Section *section, const char *newName);

// utils
bool isSectionEmpty(Section *section);
void printSection(Section *sec);
size_t getSectionSize(Section *section);

