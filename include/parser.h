#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#define MAX_BUFF_SIZE 1024
#define MAX_NUM_OF_SECTIONS 1024

typedef enum { TYPE_UNDEFINED = 0, TYPE_VALUE, TYPE_SECTION, TYPE_COMMENT } IniLineType;

typedef struct IniLine {
    char *name;
    char *value;
    IniLineType type;
    struct IniLine *nextLineInSection;
} IniLine;

typedef struct {
    char *displayName;
    uuid_t uuid;
    IniLine *firstLineInSection;
} Section;

static Section configuration[MAX_NUM_OF_SECTIONS];

Section **loadFile(const char *filename);
Section *parseFile(const char *filename, char *target, char **next);

// CRUD create
Section *newSection(char *name);

IniLine *addLineInSection(Section *section, IniLine *line);
IniLine *newLineInSection(Section *section, char *name, char *value);

void appendLineToSection(Section *section, IniLine *line);

// CRUD read
Section *getSectionByName(const char *name);
Section *getSection(Section *section);

IniLine *getLineByIndex(Section *section, int index);
IniLine *getLineByName(Section *section, char *name);

// CRUD updates
void updateSectionName(Section *section, const char *newDisplayName);
void updateLineValue(Section *section, IniLine *line, const char *newValue);

// CRUD delete
bool deleteSectionByName(const char *name);
bool deleteSection(Section *section);

bool deleteLineByName(Section *section, const char *name);
bool deleteLine(Section *section, IniLine *line);

// UTILS
bool isConfgirationValid(void);
bool isSectionEmpty(Section *section);
bool isSectionEmptyByIndex(int index);
int countLinesInSection(const Section *section);
void printSection(Section sec);

// uuid utils
bool areUUIDsEqual(uuid_t uuid1, uuid_t uuid2);
