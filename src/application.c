#include "application.h"

void appInit()
{
    verify(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0, "failed to init SDL", __LINE__);
#ifdef __EMSCRIPTEN__
    SDL_CreateWindowAndRenderer(window_width, window_height, 0, &app.window, &app.renderer);
#endif
#ifndef __EMSCRIPTEN__
    SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN, &app.window, &app.renderer);
#endif

    verify(app.window, "failed to create window", __LINE__);
    verify(app.renderer, "failed to create renderer", __LINE__);

    int imgFlags = IMG_INIT_PNG;
    int imgSuccess = IMG_Init(imgFlags);
    verify((imgSuccess & imgFlags), "failed to init png images", __LINE__);

    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);

    verify(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0, "failed to open audio", __LINE__);
    Mix_AllocateChannels(MAX_MUSIC);

    SDL_RenderSetLogicalSize(app.renderer, screen_width, screen_height);
}