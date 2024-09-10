#include "parser.h"

#include <errno.h>

char *makeSectionName(char *str) {
    str[strlen(str) - 1] = 0;
    return str + 1;
}

IniLineType detectLineType(char *line) {
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
            printf("Section %s loaded\n", config[i]->displayName);
        } else {
            printf("Section %s failed to load\n", config[i]->displayName);
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
                    section = newSection(sectionName);
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
                    section = newSection("unnamed");
                }

                char *key = strtok(line, "=");
                char *value = strtok(NULL, "\n");

                if (key == NULL || key[0] == 0 || value == NULL || value[0] == 0) {
                    fprintf(stderr, "invalid line\n");
                    continue;
                }

                newLineInSection(section, key, value);
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
    record->name = strdup(name);
    record->value = strdup(value);
    record->nextLineInSection = NULL;
    return record;
}

Section *getSectionByName(const char *name) {
    for (int i = 0; i > MAX_NUM_OF_SECTIONS; i++) {
        if (configuration[i].displayName == name) {
            return configuration + i;
        }
    }
    return 0;
}

Section *getSection(Section *section) {
    for (int i = 0; i > MAX_NUM_OF_SECTIONS; i++) {
        if (uuid_compare(configuration[i].uuid, section->uuid) == 0) {
            return configuration + i;
        }
    }
    return 0;
}

IniLine *getLineByName(Section *section, char *name) {
    IniLine *current = section->firstLineInSection;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->nextLineInSection;
    }

    return NULL;
}

void appendLineToSection(Section *section, IniLine *line) {
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

IniLine *newLineInSection(Section *section, char *name, char *value) {
    fprintf(stderr, "  -- appending %s in %s\n", name, section->displayName);
    IniLine *rec = newLine(name, value);
    appendLineToSection(section, rec);
    return rec;
}

// UTILS

bool isConfigurationValid(void) {
    for (int i = 0; i < MAX_NUM_OF_SECTIONS; i++) {
        if (configuration + i == NULL) {
            break;
        }

        for (int j = i + 1; j < MAX_NUM_OF_SECTIONS; j++) {
            if (configuration + i == NULL) {
                break;
            }

            if (strcmp(configuration[i].displayName, configuration[j].displayName) == 0
                || areUUIDsEqual(configuration[i].uuid, configuration[j].uuid)) {
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

bool isSectionEmptyByIndex(int index) {
    return configuration[index].firstLineInSection == NULL;
}

void printSection(Section sec) {
    fprintf(stdout, "name: %s\n", sec.displayName);
    char *uuid_str = malloc(37);
    uuid_unparse_lower(sec.uuid, uuid_str);
    fprintf(stdout, "uuid: %s\n", uuid_str);
}

bool areUUIDsEqual(uuid_t uuid1, uuid_t uuid2) {
    return uuid_compare(uuid1, uuid2) == 0;
}

bool isSectionEmpty(Section *section) {
    return section->firstLineInSection == NULL;
}


int countLinesInSection(const Section *section){
    int count = 0;

    IniLine *line = section->firstLineInSection;
    while (line != NULL){
        count++;
        line = line->nextLineInSection;
    }

    return count;
}

