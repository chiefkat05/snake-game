#include "snakegame.h"

Game game;

void saveGame()
{
    FILE *output;
    output = fopen("./snake.save", "wb");

    verify(output, "failed to open file for writing", __LINE__);

    verify(fwrite(&game.unlocked_levelcount, sizeof(int), 1, output), "failed to write to file", __LINE__);
    fclose(output);
}
void loadGame()
{
    FILE *input = fopen("./snake.save", "rb");
    if (!input)
    {
        game.unlocked_levelcount = 1;
        return;
    }

    verify(fread(&game.unlocked_levelcount, sizeof(int), 1, input), "failed to read from file", __LINE__);
    fclose(input);
}

int main(int argc, char **argv)
{
    srand(time(0));
    appInit();

    game.running = true;
    game.state = GAME_NULL;
    gameGoTo(GAME_MENU);

    // ran out of time during the jam to make/connect this to a sane system, so it's getting dumped at the start of the game
    game.menuMus = musicLoad("./snd/menu.ogg");
    game.grassMus = musicLoad("./snd/grass.ogg");
    game.sandboxMus = musicLoad("./snd/sandbox.ogg");
    game.volcanoMus = musicLoad("./snd/volcano.ogg");
    game.selectMus = musicLoad("./snd/select.ogg");
    game.winMus = musicLoad("./snd/won.ogg");
    game.lostMus = musicLoad("./snd/lost.ogg");
    game.explodeSnd = Mix_LoadWAV("./snd/explosion.wav");
    game.eatSnd = Mix_LoadWAV("./snd/eat.wav");
    game.moveSnd = Mix_LoadWAV("./snd/move.wav");

    loadGame();

#ifndef __EMSCRIPTEN__
    while (game.running)
    {
        gameLoop();
    }
    #endif
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(gameLoop, 0, 1);
#endif

    saveGame();

    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
    IMG_Quit();
    SDL_Quit();
    printf("goodbye!\n");
    return 0;
}