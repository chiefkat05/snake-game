#ifndef SCREEN_CURSOR_H
#define SCREEN_CURSOR_H

typedef struct
{
    int x, y;
    int activated, alert, disabled;
} ScreenCursor;

void moveCursor(ScreenCursor *cursor, int xdelta, int ydelta);
void activateCursor(ScreenCursor *cursor);
void deactivateCursor(ScreenCursor *cursor);
void disableCursor(ScreenCursor *cursor);
void enableCursor(ScreenCursor *cursor);
void alertCursor(ScreenCursor *cursor);
void settleCursor(ScreenCursor *cursor);


#endif