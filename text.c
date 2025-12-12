#include "text.h"

void fontLoad(TextFont *font, const char *path, const char *alphabet)
{
    unsigned int alength = strlen(alphabet);
    font->img = imageLoad(path);
    int x, y;

    for (y = 0; y < font->img->ycount; ++y)
    {
        for (x = 0; x < font->img->xcount; ++x)
        {
            if (font->head >= alength)
                break;

            font->glyphs[font->head] = (Glyph){alphabet[font->head], x, y};
            ++font->head;
        }
    }
}

// rough change to a hashmap or something pls
unsigned int getIndexFromChar(TextFont *font, char c)
{
    unsigned int i;
    for (i = 0; i < font->head; ++i)
    {
        if (font->glyphs[i].c == c)
        {
            return i;
        }
    }
    return 0;
}

void imageDrawText(const char *str, TextFont *font, int x, int y)
{
    unsigned int length = strlen(str);

    int i;
    int xoffset = 0;
    for (i = 0; i < length; ++i)
    {
        unsigned int index = getIndexFromChar(font, str[i]);
        imageDraw(font->img, x + xoffset, y, font->glyphs[index].tx, font->glyphs[index].ty);
        ++xoffset;
    }
}
