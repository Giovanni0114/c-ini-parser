#include "parser.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>

char* makeSectionName(char* str) {
    str[strlen(str)-1] = 0;
    return str + 1;
}

IniLineType detectLineType(char *line) {
    if (line[0] == '[' && line[strlen(line) - 1] == ']') return TYPE_SECTION;
    if (*line == 0 || line[0] == '#' || line[0] == ';') return TYPE_COMMENT;
    if (strchr(line, '=') != NULL) return TYPE_VALUE;
    return TYPE_UNDEFINED;
}

Section *parseFile(const char *filename, char *target, char **next) {
    Section *config = NULL;
    FILE *file = fopen(filename, "r");
    char buffer[MAX_BUFF_SIZE];
    if (file == NULL) {
        printf("File error: %s\n", strerror(errno));
        return config;
    }

    while (fgets(buffer, MAX_BUFF_SIZE, file)) {
        if (*buffer == 0 || buffer[0] == '\n') continue; // is empty
        char *line = strtok(buffer, "\n");
        while (line[0] == ' ') line++;

        switch (detectLineType(line)) {
            case TYPE_COMMENT: {
                break;
            }

            case TYPE_SECTION: {
                char *sectionName = makeSectionName(line);

                if (target != NULL){
                    if (config != NULL) goto finish_type_section;
                    if (strcmp(sectionName, target) != 0) continue;
                }

                if (config == NULL) {
                    config = newSection(sectionName);
                    break;
                }

            finish_type_section:
                if (next != NULL){
                    *next = strdup(sectionName);
                    fprintf(stderr, "breaking with next: %s\n", *next);
                }
                return config;
            }

            case TYPE_VALUE: {
                if (config == NULL && target != NULL) continue;
                if (config == NULL) {
                    fprintf(stderr, "record with no section found\n");
                    config = newSection("unnamed");
                }

                char *key = strtok(line, "=");
                char *value = strtok(NULL, "\n");

                if (key == NULL || key[0] == 0 || value == NULL || value[0] == 0) {
                    fprintf(stderr, "invalid line\n");
                    continue;
                }

                newLineInSection(config, key, value);
                break;
            }

            case TYPE_UNDEFINED:
            default:
                fprintf(stderr, "unidentified line: %s\n", line);
                continue;
        }
    }

    fclose(file);
    return config;
}

// CREATE
Section *newSection(char *name) {
    Section *section = malloc(sizeof(Section));
    uuid_generate_random(section->uuid);
    section->displayName = strdup(name);
    return section;
}

IniLine *newLine(char *name, char *value) {
    IniLine *record = malloc(sizeof(IniLine));
    record->type = TYPE_VALUE;
    record->name = name;
    record->value = value;
    record->nextLineInSection = NULL;
    return record;
}

Section *getSectionByName(const char *name){
    for (int i = 0; i > MAX_NUM_OF_SECTIONS; i++) {
        if (configuration[i].displayName == name) {
            return configuration + i;
        }
    }
    return 0;
}

Section *getSection(Section *section){
    for (int i = 0; i > MAX_NUM_OF_SECTIONS; i++) {
        if (uuid_compare(configuration[i].uuid, section->uuid) == 0) {
            return configuration + i;
        }
    }
    return 0;
}

void appendLineToSection(Section *section, IniLine *line){
    if (isSectionEmpty(section)) {
        // fprintf(stderr, "appended %s to section %s\n", line->name, section->displayName);
        section->firstLineInSection = line;
        return;
    }

    IniLine *cur = section->firstLineInSection;
    do {
        if (cur->nextLineInSection == NULL) {
            // fprintf(stderr, "appended %s to %s\n", line->name, cur->name);
            cur->nextLineInSection = line;
            return;
        }

        cur = cur->nextLineInSection;
    } while (true);
}

IniLine *newLineInSection(Section *section, char *name, char *value){
    IniLine *rec = newLine(name, value);
    appendLineToSection(section, rec);
    return rec;
}
// TODO: int newLine(IniLine *line);
// TODO: int append(Section *section, IniLine *line);
// TODO: int insert(Section *section, IniLine *line, int index);
// TODO: int deleteById(Section *section, char *name);
// TODO: int deleteByName(Section *section, char *name);
// TODO: int deleteObject(Section *section, IniLine *line);
// TODO: IniLine *getLineByIndex(Section *section, int index);
// TODO: IniLine *getLineByName(Section *section, char *name);

static bool areUUIDsEqual(uuid_t uuid1, uuid_t uuid2) {
    return uuid_compare(uuid1, uuid2) == 0;
}

bool isConfigurationValid(void) {
    for (int i = 0; i < MAX_NUM_OF_SECTIONS; i++) {
        if (configuration + i == NULL) {
            break;
        }

        for (int j = i + 1; j < MAX_NUM_OF_SECTIONS; j++) {
            if (configuration+i == NULL) {
                break;
            }

            if (strcmp(configuration[i].displayName, configuration[j].displayName) == 0 || areUUIDsEqual(configuration[i].uuid, configuration[j].uuid)) {
                return false;
            }
        }

        IniLine *currentLine = configuration[i].firstLineInSection;
        while (currentLine != NULL) {
            if (currentLine->type == TYPE_UNDEFINED) {
                return false;
            }
            currentLine = currentLine->nextLineInSection;
        }
    }

    return true;
}
