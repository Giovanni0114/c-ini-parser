#include "parser.h"

#include <stdlib.h>

// CREATE
Section *newSection(const char *name) {
    Section *section = malloc(sizeof(Section));
    uuid_generate_random(section->uuid);
    section->displayName[sizeof(section->displayName) - 1] = *name;
    return section;
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

// TODO: IniLine *newLineInSection(Section *section, char *name, char *value);
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
