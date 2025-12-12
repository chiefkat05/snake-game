#ifndef APPLICATION_H
#define APPLICATION_H

#include "definitions.h"
#include "audio.h"

#define window_width 1280
#define window_height 720
#define screen_width 128
#define screen_height 128

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Application;
extern Application app;

void appInit();

#endif