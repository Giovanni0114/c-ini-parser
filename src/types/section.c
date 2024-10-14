#include "types/section.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Section* createEmptySection(void) {
    return (Section*)malloc(sizeof(Section));
}

Section* createSection(const char* name) {
    Section* section = createEmptySection();

    if (!section) {
        return NULL;
    }

    strncpy(section->name, name, sizeof(name)-1);
    section->name[sizeof(section->name) - 1] = '\0';

    uuid_generate_random(section->uuid);

    return section;
}

bool isSectionEmpty(Section *section) {
    return section->firstLineInSection == NULL;
}

void appendLineToSection(Section *section, Line *line) {
    if (isSectionEmpty(section)) {
        // fprintf(stderr, "appended %s to section %s\n", line->name, section->displayName);
        section->firstLineInSection = line;
        return;
    }

    Line *cur = section->firstLineInSection;
    do {
        if (cur->nextLineInSection == NULL) {
            // fprintf(stderr, "appended %s to %s\n", line->name, cur->name);
            cur->nextLineInSection = line;
            return;
        }

        cur = cur->nextLineInSection;
    } while (true);
}

Line *getLineByName(Section *section, char *name) {
    Line *current = section->firstLineInSection;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->nextLineInSection;
    }

    return NULL;
}

void printSection(Section *sec) {
    fprintf(stdout, "name: %s\n", sec->name);
    char *uuid_str = malloc(37);
    uuid_unparse_lower(sec->uuid, uuid_str);
    fprintf(stdout, "uuid: %s\n", uuid_str);
    free(uuid_str);
}

void deleteAllLines(Section *section){
    Line *tmpLine = section->firstLineInSection;
    while (tmpLine != NULL) {
        Line *tmpLineNext = tmpLine->nextLineInSection;
        free(tmpLine);
        tmpLine = tmpLineNext;
    }
}

size_t getSectionSize(Section *section){
    size_t len = 0;
    Line *lookup = section->firstLineInSection;

    while (lookup != NULL) {
        lookup = lookup->nextLineInSection;
        len++;
    }

    return len;
}

void updateSectionName(Section *section, const char *newName){
    strncpy(section->name, newName, sizeof(section) - 1);
    section->name[sizeof(section) - 1] = '\0';
}

