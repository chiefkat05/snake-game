#include "definitions.h"
#include "graphics.h"
#include "audio.h"
#include "application.h"

#define window_width 1280
#define window_height 720
#define screen_width 128
#define screen_height 128

// game plan

// get current time from sdl and implement a platform-agnostic tick system
// remake snake graphics for all the different cases of head/body/tail movement, and apply them in the code
// make a drawmulti function that can draw a larger texture clip rect (still locked to 16 pixel values)
// make the ui map with the title screen and buttons
// make a simple button thing that runs a function on press
// make the lose screen map
// make sure the game screens are working and operational, main menu screen -> classic mode screen -> lose screen (upload builds)
// make puzzle mode button and add game screen with more hands-on board/food/snake positioning functions (only one level)
// make a different logic loop and input loop for puzzle mode
// separate game init loop and have more screen-independant variable names where applicable.
//      There will be one of every spritemap as a pointer in the game struct, and all the music too. Each screen should have
//      an init function where it sets up music/sprites/etc, and a close function where it 'frees' all the resources that it
//      was using. There will be a screen for menu, settings (if possible), win, lose, classic mode setup, classic mode, and
//      one screen for each puzzle mode level.

// animations are going to be so hard fuck

Application app;
ImagePool img_pool;
MusicPool music_pool;

typedef enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTHEAST,
    SOUTHEAST,
    NORTHWEST,
    SOUTHWEST
} Direction;
Direction getMoveDirection(int xdelta, int ydelta)
{
    if (xdelta < 0 && ydelta == 0)
        return WEST;
    if (xdelta > 0 && ydelta == 0)
        return EAST;
    if (xdelta == 0 && ydelta < 0)
        return NORTH;
    if (xdelta == 0 && ydelta > 0)
        return SOUTH;
    if (xdelta > 0 && ydelta > 0)
        return SOUTHEAST;
    if (xdelta < 0 && ydelta > 0)
        return SOUTHWEST;
    if (xdelta > 0 && ydelta < 0)
        return NORTHEAST;
    if (xdelta < 0 && ydelta < 0)
        return SOUTHWEST;

    return 0; // won't get here
}
Direction getReverseDirection(Direction direction)
{
    switch (direction)
    {
        case WEST:
            return EAST;
        case EAST:
            return WEST;
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case NORTHWEST:
            return SOUTHEAST;
        case SOUTHEAST:
            return NORTHWEST;
        case NORTHEAST:
            return SOUTHWEST;
        case SOUTHWEST:
            return NORTHEAST;
        default:
            return 0; // won't get here
    }
    return 0; // won't get here either
}
int getXNormalFromDirection(Direction direction)
{
    switch(direction)
    {
        case WEST:
        case NORTHWEST:
        case SOUTHWEST:
            return -1;
        case EAST:
        case NORTHEAST:
        case SOUTHEAST:
            return 1;
        case NORTH:
        case SOUTH:
        default:
            return 0;
    }
    return 0;
}
int getYNormalFromDirection(Direction direction)
{
    switch(direction)
    {
        case NORTH:
        case NORTHWEST:
        case NORTHEAST:
            return -1;
        case SOUTH:
        case SOUTHWEST:
        case SOUTHEAST:
            return 1;
        case WEST:
        case EAST:
        default:
            return 0;
    }
    return 0;
}

#define MAX_FOODS 16
typedef struct
{
    int x, y;
    int eaten;
} Food;

#define MAX_SNAKEPARTS 64
#define MOVE_TIMER_RESET 5000.0
typedef struct
{
    int x, y;
    Direction last_move_direction;
} SnakePart;
typedef struct
{
    SnakePart parts[MAX_SNAKEPARTS];
    int length;
    Direction next_move_direction;
    double move_timer;
} Snake;

#define BOARD_WIDTH 16
#define BOARD_HEIGHT 16
typedef struct
{
    int texture[BOARD_WIDTH][BOARD_HEIGHT][2];
    int width, height;

    int left, right, top, bottom;
} Board;

typedef enum
{
    GAME_MENU,
    GAME_SELECT,
    GAME_PUZZLE,
    GAME_SETTINGS,
    GAME_CLASSIC,
    GAME_LOST,
    GAME_WON
} GameState;

typedef struct
{
    int running;
    GameState state;
    int level;

    Snake snake;
    Food foods[MAX_FOODS];
    int foodcount;

    Board board;

    Image *boardImg;
    Image *snakeImg;
    Image *uiImg;

    Music *lowMus;
    Music *highMus;
} Game;
static Game game;

typedef enum
{
    KEY_RELEASED,
    KEY_HELD,
    KEY_TAPPED
} KeyState;
static KeyState keystates[SDL_NUM_SCANCODES];

KeyState getKey(SDL_Scancode key)
{
    return keystates[key];
}

void putSnake(int x, int y)
{
    game.snake.parts[0].x = x;
    game.snake.parts[0].y = y;

    int i;
    for (i = game.snake.length; i > 0; --i)
    {
        game.snake.parts[i].x = game.snake.parts[0].x - i;
        game.snake.parts[i].y = game.snake.parts[0].y;
    }
}
void makeBoard(int width, int height)
{
    game.board.width = width;
    game.board.height = height;

    game.board.left = (BOARD_WIDTH - game.board.width) / 2;
    game.board.top = (BOARD_HEIGHT - game.board.height) / 2;
    game.board.right = game.board.width + game.board.left;
    game.board.bottom = game.board.height + game.board.top;
    
}
void spawnFood(int foodnum)
{
    int i;
    for (i = 0; i < foodnum; ++i)
    {
        game.foods[i].x = rand() % (game.board.width - 2) + game.board.left + 1;
        game.foods[i].y = rand() % (game.board.height - 2) + game.board.top + 1;

        game.foods[i].eaten = 0;
    }
    game.foodcount = foodnum;
}
void gameInit()
{
    game.state = GAME_CLASSIC;
    game.running = true;
    game.level = 0;
    game.snake.length = 1;
    game.snake.parts[0].last_move_direction = EAST;
    game.snake.next_move_direction = EAST;
    game.snake.move_timer = MOVE_TIMER_RESET;
    putSnake(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);
    makeBoard(10, 10);

    spawnFood(5);
}

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
    gameInit();

    // get current time
    prev_time = current_time;
    current_time = SDL_GetTicks();
}
void gameUpdateTime()
{
    prev_time = current_time;
    current_time = SDL_GetTicks();
    frame_time = current_time - prev_time;
    accumulated_time += frame_time;
}
int gameCaughtUp()
{
    if (accumulated_time >= tick_speed)
    {
        accumulated_time -= tick_speed;
        return 0;
    }
    return 1;
}

void moveSnake(int xdelta, int ydelta)
{
    if (getMoveDirection(xdelta, ydelta) == getReverseDirection(game.snake.parts[0].last_move_direction))
        return;
    
    int i;
    for (i = game.snake.length; i > 0; --i)
    {
        int partxdelta = game.snake.parts[i - 1].x - game.snake.parts[i].x;
        int partydelta = game.snake.parts[i - 1].y - game.snake.parts[i].y;
        game.snake.parts[i].x = game.snake.parts[i - 1].x;
        game.snake.parts[i].y = game.snake.parts[i - 1].y;
        game.snake.parts[i].last_move_direction = getMoveDirection(partxdelta, partydelta);
    }

    game.snake.parts[0].x += xdelta;
    game.snake.parts[0].y += ydelta;

    game.snake.parts[0].last_move_direction = getMoveDirection(xdelta, ydelta);
}
void drawSnake()
{
    // change the snake sprite based on snake.last_move_direction
    int i;
    for (i = 0; i < game.snake.length; ++i)
    {
        if (game.snake.parts[i].x == 0 && game.snake.parts[i].y == 0)
        {
            printf("hello\n");
        }
        if (i == 0)
            imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 2, 2);
        else if (i == game.snake.length - 1)
            imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 0, 2);
        else
            imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 1, 2);
    }
}
void updateFood()
{
    int i;
    int alleaten = 1;
    for (i = 0; i < game.foodcount; ++i)
    {
        if (!game.foods[i].eaten)
        {
            alleaten = 0;
            break;
        }
    }

    if (alleaten)
    {
        spawnFood(game.foodcount);
    }
}
void drawFood()
{
    int i;
    for (i = 0; i < game.foodcount; ++i)
    {
        if (game.foods[i].eaten)
            continue;

        imageDraw(game.boardImg, game.foods[i].x, game.foods[i].y, 2, 1);
    }
}
void drawBoard()
{
    // remember 0 is top and 16 or something is bottom
    int x, y;
    for (y = game.board.top; y < game.board.bottom; ++y)
    {
        for (x = game.board.left; x < game.board.right; ++x)
        {
            if (x == game.board.left || x == game.board.right - 1 || y == game.board.top || y == game.board.bottom - 1)
            {
                imageDraw(game.boardImg, x, y, 0, 0);
            }
            else
            {
                imageDraw(game.boardImg, x, y, 2, 0);
            }
        }
    }
}

void gameRules()
{
    int i;

    if (game.snake.parts[0].x == game.board.left ||
        game.snake.parts[0].x == game.board.right - 1 ||
        game.snake.parts[0].y == game.board.top ||
        game.snake.parts[0].y == game.board.bottom - 1)
    {
        game.state = GAME_LOST;
    }

    for (i = 1; i < game.snake.length; ++i)
    {
        if (game.snake.parts[0].x == game.snake.parts[i].x &&
            game.snake.parts[0].y == game.snake.parts[i].y)
        {
            game.state = GAME_LOST;
        }
    }

    for (i = 0; i < game.foodcount; ++i)
    {
        if (!game.foods[i].eaten &&
            game.snake.parts[0].x == game.foods[i].x &&
            game.snake.parts[0].y == game.foods[i].y)
        {
            game.foods[i].eaten = 1;
            game.snake.parts[game.snake.length].x = game.snake.parts[game.snake.length - 1].x - getXNormalFromDirection(game.snake.parts[game.snake.length - 1].last_move_direction);
            game.snake.parts[game.snake.length].y = game.snake.parts[game.snake.length - 1].y - getYNormalFromDirection(game.snake.parts[game.snake.length - 1].last_move_direction);
            ++game.snake.length;
        }
    }

    game.snake.move_timer -= tick_speed * (1.0 + (double)game.snake.length / 10.0);

    // would allow player sideways movement to be instant
    // if (game.snake.move_timer <= 0.0 ||
    //     (game.snake.next_move_direction != game.snake.parts[0].last_move_direction &&
    //      game.snake.next_move_direction != getReverseDirection(game.snake.parts[0].last_move_direction)))
    if (game.snake.move_timer <= 0.0)
    {
        game.snake.move_timer = MOVE_TIMER_RESET;
        if (game.snake.parts[0].last_move_direction == getReverseDirection(game.snake.next_move_direction))
            game.snake.next_move_direction = game.snake.parts[0].last_move_direction;
        moveSnake(getXNormalFromDirection(game.snake.next_move_direction), getYNormalFromDirection(game.snake.next_move_direction));
    }
}

void sdlEventLoop(SDL_Event *event)
{
    while(SDL_PollEvent(event))
    {
        switch(event->type)
        {
            case SDL_QUIT:
                game.running = false;
                break;
            case SDL_MOUSEMOTION:
                if (event->motion.x < 0 || event->motion.x > screen_width || event->motion.y < 0 || event->motion.y > screen_height)
                    continue;
                // printf("mouse moved to %i %i\n", event->motion.x, event->motion.y);
                break;
            case SDL_KEYDOWN:
                if (event->key.repeat)
                    continue;

                if (keystates[event->key.keysym.scancode] == KEY_RELEASED)
                {
                    keystates[event->key.keysym.scancode] = KEY_TAPPED;
                    continue;
                }
                keystates[event->key.keysym.scancode] = KEY_HELD;
                break;
            case SDL_KEYUP:
                keystates[event->key.keysym.scancode] = KEY_RELEASED;
                break;
            default:
                break;
        }

    }
}
void handleInput()
{
    if (getKey(SDL_SCANCODE_W) == KEY_TAPPED || getKey(SDL_SCANCODE_UP) == KEY_TAPPED)
    {
        game.snake.next_move_direction = NORTH;
    }
    if (getKey(SDL_SCANCODE_S) == KEY_TAPPED || getKey(SDL_SCANCODE_DOWN) == KEY_TAPPED)
    {
        game.snake.next_move_direction = SOUTH;
    }
    if (getKey(SDL_SCANCODE_A) == KEY_TAPPED || getKey(SDL_SCANCODE_LEFT) == KEY_TAPPED)
    {
        game.snake.next_move_direction = WEST;
    }
    if (getKey(SDL_SCANCODE_D) == KEY_TAPPED || getKey(SDL_SCANCODE_RIGHT) == KEY_TAPPED)
    {
        game.snake.next_move_direction = EAST;
    }
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
    }
}

static int mouseX, mouseY;
static SDL_Event event;
void game_loop()
{
    SDL_RenderClear(app.renderer);

    updateKeys();
    sdlEventLoop(&event);
    handleInput();

    int game_caught_up = 0;
    gameUpdateTime();

    switch(game.state)
    {
    case GAME_MENU:
        // menu here
        break;
    case GAME_CLASSIC:

        while (!game_caught_up)
        {
            gameRules();
            updateFood();
            musicFadeIn(game.lowMus);
            musicFadeIn(game.highMus);
            game_caught_up = gameCaughtUp();
        }

        drawBoard();
        drawFood();
        drawSnake();
        imageDraw(game.uiImg, 15, 15, 1, 0);

        SDL_GetMouseState(&mouseX, &mouseY);
    break;
    case GAME_LOST:
        musicFadeOut(game.lowMus);
        // printf("you lose, loser\n");
        // lost graphic here
        break;
    default:
        break;
    }

    SDL_RenderPresent(app.renderer);
}

int main(int argc, char **argv)
{
    srand(time(0));
    appInit();

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
        game_loop();
    }
    #endif
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(game_loop, 0, 1);
#endif


    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
    IMG_Quit();
    SDL_Quit();
    printf("goodbye!\n");
    return 0;
}