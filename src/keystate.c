#include "keystate.h"

KeyState getKey(SDL_Scancode key)
{
    return keystates[key];
}
int getAnyKey()
{
    if (anyKeyPressed > 0)
    {
        return 1;
    }
    return 0;
}
void updateKeys()
{
    int i;
    for (i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        if (keystates[i] == KEY_TAPPED)
        {
            keystates[i] = KEY_HELD;
        }
        if (keystates[i] == KEY_JUSTRELEASED)
        {
            keystates[i] = KEY_RELEASED;
        }
    }
}