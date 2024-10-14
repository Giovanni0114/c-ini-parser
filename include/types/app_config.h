#pragma once

#define CONFIG_FILE_PATH "config.ini"

typedef struct {
    int port;
    char **ensureExist;
    char *path;
} AppConfig;

AppConfig *createEmptyAppConfig(void);
AppConfig *createDefaultAppConfig(void);

void initConfig(AppConfig *config);
void freeConfig(AppConfig *config);

// TODO: here to add -c option to console for read config
void setDefaultConfig(void);

void setPort(int port);
int getPort(void);

void setEnsureExists(char **sectionsToEnsure);
void addEnsureExists(char *sectionToEnsure);
char **getEnsureExists(void);
