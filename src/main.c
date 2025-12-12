#include "snakegame.h"

Game game;

int main(int argc, char **argv)
{
    srand(time(0));
    appInit();

    game.running = true;
    game.state = GAME_WON;
    game.unlocked_levelcount = 1; // get from save file
    gameGoTo(GAME_MENU);

#ifndef __EMSCRIPTEN__
    while (game.running)
    {
        gameLoop();
    }
    #endif
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(gameLoop, 0, 1);
#endif


    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
    IMG_Quit();
    SDL_Quit();
    printf("goodbye!\n");
    return 0;
}