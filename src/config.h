typedef enum {
    NONE = 0,
    AUTO,
    PERIODICLY,
} VaccumMode;

typedef struct {
    VaccumMode VACCUM_MODE;
} Config;

Config config;

Config defaultConfig(void);
void setDefaultConfig(void);

void setVACCUM_MODE(VaccumMode mode);
VaccumMode getVACCUM_MODE(void);
