#include "types/section.h"

typedef struct Config {
    char *name;
    Section *firstSection;
} Config;

// constructor
Config *createEmptyConfig(void);
Config *createConfig(const char* name);

// get child 
Section *getSectionByUuid(Config *config, uuid_t uuid);
Section *getSectionByName(Config *config, const char *name);
Section *getSectionAt(Config *config, int idx);
Section *getLastSection(Config *config);

// add child
void appendSection(Config *config, Section *section);
bool insertSection(Config *config, Section *section, int idx);

//  delete child
bool deleteSectionByName(Config *config, const char *name);
bool deleteSectionByUuid(Config *config, const uuid_t uuid);
bool deleteSectionAt(Config *config, int idx);

void deleteAllSections(Config *config);

//  update
void updateConfigName(Config *config, const char *newDisplayName);

// utils
bool isConfigValid(Config *config);
bool isConfigEmpty(Config *config);
size_t getConfigSize(Config *config);
