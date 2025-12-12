#ifndef KEYSTATE_H
#define KEYSTATE_H

#include "definitions.h"

typedef enum
{
    KEY_RELEASED,
    KEY_HELD,
    KEY_TAPPED
} KeyState;
extern KeyState keystates[SDL_NUM_SCANCODES];
extern int anyKeyPressed;

KeyState getKey(SDL_Scancode key);
int getAnyKey();

#endif