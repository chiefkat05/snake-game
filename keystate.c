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