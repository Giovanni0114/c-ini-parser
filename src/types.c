typedef enum {
    TYPE_UNDEFINED = 0,
    TYPE_VALUE,
    TYPE_SECTION,
    TYPE_COMMENT
} IniLineType;

typedef struct {
    char *name;
    IniLineType type;
} IniLine;
