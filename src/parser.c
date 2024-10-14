#include "parser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *makeSectionName(char *str) {
    str[strlen(str) - 1] = 0;
    return str + 1;
}

LineType detectLineType(char *line) {
    if (line[0] == '[' && line[strlen(line) - 1] == ']') return TYPE_SECTION;
    if (*line == 0 || line[0] == '#' || line[0] == ';') return TYPE_COMMENT;
    if (strchr(line, '=') != NULL) return TYPE_VALUE;
    return TYPE_UNDEFINED;
}

int countSection(const char *filename) {
    int count = 0;
    FILE *file = fopen(filename, "r");
    char buffer[MAX_BUFF_SIZE];
    if (file == NULL) {
        printf("File error: %s\n", strerror(errno));
        return 0;
    }

    while (fgets(buffer, MAX_BUFF_SIZE, file)) {
        if (*buffer == 0 || buffer[0] == '\n') continue;  // is empty
        char *line = strtok(buffer, "\n");
        while (line[0] == ' ') line++;
        if (detectLineType(line) == TYPE_SECTION) count++;
    }
    return count;
}

Section **loadFile(const char *filename) {
    int numOfSection = countSection(filename);
    Section **config = malloc(sizeof(Section *) * numOfSection);

    printf("numOfSection: %d\n", numOfSection);
    if (numOfSection == 0) return config;

    char *next = NULL;
    for (int i = 0; i < numOfSection; i++) {
        char *target = NULL;
        if (next != NULL) target = strdup(next);
        next = NULL;
        config[i] = parseFile(filename, target, &next);

        if (config[i] != NULL) {
            printf("Section %s loaded\n", config[i]->name);
        } else {
            printf("Section %s failed to load\n", config[i]->name);
        }
        free(target);
    }
    free(next);

    return config;
}

Section *parseFile(const char *filename, char *target, char **next) {
    Section *section = NULL;
    FILE *file = fopen(filename, "r");
    char buffer[MAX_BUFF_SIZE];
    if (file == NULL) {
        printf("File error: %s\n", strerror(errno));
        return section;
    }

    while (fgets(buffer, MAX_BUFF_SIZE, file)) {
        if (*buffer == 0 || buffer[0] == '\n') continue;  // is empty
        char *line = strtok(buffer, "\n");
        while (line[0] == ' ') line++;

        switch (detectLineType(line)) {
            case TYPE_COMMENT: {
                break;
            }

            case TYPE_SECTION: {
                char *sectionName = makeSectionName(line);

                if (target != NULL) {
                    if (section != NULL) goto finish_type_section;
                    if (strcmp(sectionName, target) != 0) continue;
                }

                if (section == NULL) {
                    section = createSection(sectionName);
                    break;
                }

            finish_type_section:
                if (next != NULL) {
                    *next = strdup(sectionName);
                }
                return section;
            }

            case TYPE_VALUE: {
                if (section == NULL && target != NULL) continue;
                if (section == NULL) {
                    fprintf(stderr, "record with no section found\n");
                    section = createSection("unnamed");
                }

                char *key = strtok(line, "=");
                char *value = strtok(NULL, "\n");

                if (key == NULL || key[0] == 0 || value == NULL || value[0] == 0) {
                    fprintf(stderr, "invalid line\n");
                    continue;
                }

                // newLineInSection(section, key, value);
                Line *new = createLine(key, value);
                appendLineToSection(section, new);
                break;
            }

            case TYPE_UNDEFINED:
            default:
                fprintf(stderr, "unidentified line: %s\n", line);
                continue;
        }
    }

    fclose(file);
    return section;
}

Line *newLineInSection(Section *section, char *name, char *value) {
    // fprintf(stderr, "  -- appending %s in %s\n", name, section->displayName);
    Line *rec = createLine(name, value);
    appendLineToSection(section, rec);
    return rec;
}

// UTILS

int countLinesInSection(const Section *section){
    int count = 0;

    Line *line = section->firstLineInSection;
    while (line != NULL){
        count++;
        line = line->nextLineInSection;
    }

    return count;
}

