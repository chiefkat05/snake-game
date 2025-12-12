#include "screencursor.h"

void moveCursor(ScreenCursor *cursor, int xdelta, int ydelta)
{
    cursor->x += xdelta;
    cursor->y += ydelta;
}
void activateCursor(ScreenCursor *cursor)
{
    if (!cursor->disabled && cursor->alert)
    {
        cursor->activated = 1;
        cursor->alert = 0;
    }
}
void deactivateCursor(ScreenCursor *cursor)
{
    cursor->activated = 0;
}
void disableCursor(ScreenCursor *cursor)
{
    cursor->disabled = 1;
    cursor->alert = 0;
    cursor->activated = 0;
}
void enableCursor(ScreenCursor *cursor)
{
    cursor->disabled = 0;
}
void alertCursor(ScreenCursor *cursor)
{
    cursor->alert = 1;
}
void settleCursor(ScreenCursor *cursor)
{
    cursor->alert = 0;
}