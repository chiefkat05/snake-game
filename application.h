#ifndef APPLICATION_H
#define APPLICATION_H

#include "definitions.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Application;
extern Application app;

#endif