#pragma once

#define CONFIG_FILE_PATH "config.ini"
typedef enum {
    V_MODE_NONE = 0,
    V_MODE_AUTO,
    V_MODE_PERIODICLY,
} VaccumMode;

typedef enum {
    V_PERIOD_NONE = 0,
    V_PERIOD_DAILY,
    V_PERIOD_WEEKLY,
    V_PERIOD_MONTHLY,
} VaccumModePeriod;

typedef struct {
    VaccumMode vaccumMode;
    VaccumModePeriod vaccumModePeriod;
    int port;
    char **ensureExist;
    char *path;
} Config;

void initConfig(Config *config);
void freeConfig(Config *config);

// idea here to add -c option to console for read config
// and also -f option to override default config
// TODO: for future implementation P=1
void setDefaultConfig(void);

void setVaccumModePeriod(VaccumModePeriod period);
VaccumMode getVaccumModePeriod(void);

void setVaccumMode(VaccumMode mode);
VaccumMode getVaccumMode(void);

void setPort(int port);
int getPort(void);

void setEnsureExists(char **sectionsToEnsure);
void addEnsureExists(char *sectionToEnsure);
char **getEnsureExists(void);
