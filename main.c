#include "definitions.h"
#include "graphics.h"
#include "audio.h"
#include "application.h"

#define window_width 320
#define window_height 320

// game plan

// add a 'board' 2d array
// snake object with a 2d position
// player input moves the 2d position of the snake
// draw a small texture at the snake 2d position
// snake parts that are set to the snake 'head' position on move, limited by the snake's 'length' value
// draw more sprites on the snake parts
// if snake head position is on snake part or on the edge of the board, game exits (print 'you lose' or somthng)
// food item that increases snake length and sets itself to 'eaten' when snake head is on top of

Application app;
ImagePool img_pool;
MusicPool music_pool;

typedef enum
{
    GSTATE_MENU,
    GSTATE_LEVELSELECT,
    GSTATE_SETTINGS,
    GSTATE_GAME,
    GSTATE_LOST,
    GSTATE_WON
} GameState;

typedef struct
{
    int running;
    GameState state;
    int level;

    Image *boardImg;
    Image *snakeImg;
    Image *uiImg;

    Music *lowMus;
    Music *highMus;
} Game;
static Game game;

void gameInit()
{
    game.state = GSTATE_MENU;
    game.running = true;
    game.level = 0;
}

void appInit()
{
    verify(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0, "failed to init SDL", __LINE__);

    SDL_CreateWindowAndRenderer(window_width, window_height, 0, &app.window, &app.renderer);
    app.min_window_side = window_height;

    verify(app.window, "failed to create window", __LINE__);
    verify(app.renderer, "failed to create renderer", __LINE__);

    int imgFlags = IMG_INIT_PNG;
    int imgSuccess = IMG_Init(imgFlags);
    verify((imgSuccess & imgFlags), "failed to init png images", __LINE__);

    SDL_SetRenderDrawColor(app.renderer, 170, 240, 255, 255);

    Game game;
    game.running = true;
    gameInit();

    SDL_Rect game_view = {.x = 0, .y = 0, .w = 90, .h = 120}, ui_view = {.x = 90, .y = 0, .w = 30, .h = 120};
}

void event_loop(Game *game, SDL_Event *event)
{
    while(SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            game->running = false;
        }
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_w)
        {
            printf("move up\n");
        }
        if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
        {
            // lol
            app.min_window_side = event->window.data1 * (event->window.data1 < event->window.data2) +
                            event->window.data2 * (event->window.data2 < event->window.data1);
        }
    }
}

#define MAX_SNAKEPARTS 64
typedef struct
{
    int x, y;
} SnakePart;
typedef struct
{
    SnakePart parts[MAX_SNAKEPARTS];
    int length;
} Snake;

static int mouseX, mouseY;
static SDL_Event event;
static int thing;
void game_loop(void *game_data)
{
    Game *data = (Game *)game_data;

    if (thing == 0)
    {
        thing = 4;
        // data->lowMusc
    }
    
    event_loop(&game, &event);

    SDL_RenderClear(app.renderer);

    SDL_RenderSetLogicalSize(app.renderer, app.min_window_side, app.min_window_side);
    imageDraw(data->snakeImg, 40, 40);
    imageDraw(data->boardImg, 10, 10);
    imageDraw(data->boardImg, 20, 10);
    imageDraw(data->boardImg, 30, 10);
    imageDraw(data->boardImg, 40, 10);
    imageDraw(data->boardImg, 10, 20);
    imageDraw(data->boardImg, 20, 20);
    imageDraw(data->boardImg, 30, 20);
    imageDraw(data->boardImg, 40, 20);
    imageDraw(data->uiImg, 0, 0);

    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX < 100)
    {
        musicFadeIn(data->lowMus);
        musicFadeOut(data->highMus);
    }
    else if (mouseX > window_width - 100)
    {
        musicFadeIn(data->lowMus);
        musicFadeIn(data->highMus);
    }
    else
    {
        musicFadeOut(data->lowMus);
        musicFadeIn(data->highMus);
    }

    SDL_RenderPresent(app.renderer);
}

int main(int argc, char **argv)
{
    appInit();

    verify(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) >= 0, "failed to open audio", __LINE__);
    Mix_AllocateChannels(MAX_MUSIC);

    game.snakeImg = imageLoad("./img/snakemap.png");
    game.boardImg = imageLoad("./img/grassmap.png");
    game.uiImg = imageLoad("./img/uimap.png");

    game.lowMus = musicLoad("./snd/testsync.ogg");
    game.highMus = musicLoad("./snd/testsync2.ogg");

    musicPlay(game.lowMus, 0.16);
    musicPlay(game.highMus, 0.16);

#ifndef __EMSCRIPTEN__
    while (game.running)
    {
        game_loop(&game);
    }
    #endif
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(game_loop, &game, 0, 1);
#endif


    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
    IMG_Quit();
    SDL_Quit();
    printf("goodbye!\n");
    return 0;
}