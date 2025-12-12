#ifndef TEXT_H
#define TEXT_H

#include "definitions.h"
#include "graphics.h"

typedef struct
{
    char c;
    int tx, ty;
} Glyph;

#define MAX_TEXT_GLYPHS 256
typedef struct
{
    Glyph glyphs[MAX_TEXT_GLYPHS];
    int head;
    Image *img;
} TextFont;

void fontLoad(TextFont *font, const char *path, const char *alphabet);

// rough change to a hashmap or something pls
unsigned int getIndexFromChar(TextFont *font, char c);

void imageDrawText(const char *str, TextFont *font, int x, int y);

#endif