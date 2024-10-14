#include "types/config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Config *createEmptyConfig(void) {
    return (Config *)malloc(sizeof(Config));
}
Config *createConfig(const char *name) {
    Config *config = createEmptyConfig();

    strncpy(config->name, name, sizeof(config->name) - 1);
    config->name[sizeof(config->name) - 1] = '\0';

    return config;
}

/**
 * @deprecated This function is functional but not recommended for use.
 * Please use `getSectionByUuid()` instead as it is safer.
 *
 * Reason: Section names are not always unique, this function will return pointer to FIRST section of fiven name
 */
Section *getSectionByName(Config *config, const char *name) {
    Section *lookup = config->firstSection;

    while (lookup != NULL) {
        if (strcmp(lookup->name, name) == 0) {
            return lookup;
        }
        lookup = lookup->nextSectionInConfig;
    }

    return NULL;
}

Section *getSectionByUuid(Config *config, uuid_t uuid) {
    Section *lookup = config->firstSection;

    while (lookup != NULL) {
        if (uuid_compare(lookup->uuid, uuid) == 0) {
            return lookup;
        }
        lookup = lookup->nextSectionInConfig;
    }

    return NULL;
}

/**
 * @deprecated This function is functional but not recommended for use.
 * Please use `deleteSectionByUuid()` instead as it is safer.
 *
 * Reason: Section names are not always unique, this function will delete pointer to FIRST section of fiven name
 */
bool deleteSectionByName(Config *config, const char *name) {
    Section *lookup = config->firstSection;

    if (lookup == NULL) {
        return false;
    }

    while (lookup->nextSectionInConfig != NULL) {
        if (strcmp(lookup->nextSectionInConfig->name, name) == 0) {
            return true;
        }

        lookup = lookup->nextSectionInConfig;
    }

    return false;
}

bool deleteSectionByUuid(Config *config, const uuid_t uuid) {
    Section *lookup = config->firstSection;

    if (lookup == NULL) {
        return false;
    }

    while (lookup->nextSectionInConfig != NULL) {
        if (uuid_compare(lookup->nextSectionInConfig->uuid, uuid) == 0) {
            return true;
        }

        lookup = lookup->nextSectionInConfig;
    }

    return false;
}

Section *getSectionAt(Config *config, int idx) {
    if (idx == -1) {
        return getLastSection(config);
    }

    Section *lookup = config->firstSection;
    for (; idx > 0; idx--) {
        if (lookup == NULL) {
            return NULL;
        }
        lookup = lookup->nextSectionInConfig;
    }

    return lookup;
}

bool deleteSectionAt(Config *config, int idx) {
    if (idx == 0) {
        Section *tmp = config->firstSection;
        config->firstSection = tmp->nextSectionInConfig;
        // free all Lines in Section
        Line *tmpLine = tmp->firstLineInSection;
        while (tmpLine != NULL) {
            Line *tmpLineNext = tmpLine->nextLineInSection;
            free(tmpLine);
            tmpLine = tmpLineNext;
        }
        free(tmp);
        return true;
    }

    if (idx < 0) {
        assert(0 && "unimplemented");
        return true;
    }

    if (idx > (int)getConfigSize(config)) {
        return false;
    }

    Section *target = getSectionAt(config, idx);
    Section *prev = getSectionAt(config, idx - 1);

    if (target == NULL || prev == NULL) {
        return false;
    }

    prev->nextSectionInConfig = target->nextSectionInConfig;
    free(target);

    return true;
}

Section *getLastSection(Config *config) {
    Section *lookup = config->firstSection;

    if (lookup == NULL) {
        return NULL;
    }

    while (lookup->nextSectionInConfig != NULL) {
        lookup = lookup->nextSectionInConfig;
    }

    return lookup;
}

void appendSection(Config *config, Section *section) {
    Section *last = getLastSection(config);

    if (last == NULL) {
        config->firstSection = section;
        return;
    }

    last->nextSectionInConfig = section;
}

// utils
bool isConfigValid(Config *config) {
    // TODO: Do something
    (void)config;  // unused
    return true;
}

bool isConfigEmpty(Config *config) {
    return config->firstSection == NULL;
}

size_t getConfigSize(Config *config) {
    size_t len = 0;
    Section *lookup = config->firstSection;

    while (lookup != NULL) {
        lookup = lookup->nextSectionInConfig;
        len++;
    }

    return len;
}

void updateConfigName(Config *config, const char *newDisplayName) {
    strncpy(config->name, newDisplayName, sizeof(config) - 1);
    config->name[sizeof(config) - 1] = '\0';
}

bool insertSection(Config *config, Section *section, int idx) {
    Section *target = getSectionAt(config, idx);

    if (target == NULL) {
        return false;
    }

    section->nextSectionInConfig = target->nextSectionInConfig;
    target->nextSectionInConfig = section;
    return true;
}

void deleteAllSections(Config *config){
    while (!isConfigEmpty(config)) {
        deleteSectionAt(config, 0);
    }
}
