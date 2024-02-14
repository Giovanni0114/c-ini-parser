
:xor:typedef enum {
    TYPE_UNDEFINED = 0,
    TYPE_VALUE,
    // TYPE_VALUE_WITH_COMMENT,
    TYPE_SECTION,
    // TYPE_SECTION_WITH_COMMENT,
    TYPE_COMMENT
} IniLineType;

typedef struct {
    char *name;
    char *value;
    IniLineType type;
} IniLine;
