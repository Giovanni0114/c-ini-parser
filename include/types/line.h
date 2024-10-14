#pragma once

typedef struct Line {
    char *name;
    char *value;
    struct Line *nextLineInSection;
} Line;

Line *createEmptyLine(void);
Line *createLine(const char* name, const char* value);
