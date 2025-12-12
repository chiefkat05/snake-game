#include "definitions.h"
#include "graphics.h"
#include "audio.h"
#include "application.h"
#include "animation.h"
#include "direction.h"
#include "keystate.h"
#include "text.h"

#define window_width 1280
#define window_height 720
#define screen_width 128
#define screen_height 128

// game plan

// make puzzle mode button and add puzzle 1-1 game screen with more hands-on board/food/snake positioning functions (only one level)
// make a different logic loop and input loop for puzzle mode
// make a win condition for 1-1 and then do it again two more times for three total grass levels
// make the puzzle select screen with a different play button to access from the main menu
// have little icons for the three puzzle levels, and greyed out versions for not-unlocked-yet.
// make it so that winning a level boots you to the level select screen (after a win animation? just same as lose animation?)
//      and unlocks the next level (with a little animation if you're willing after you confirm it works)

Application app;
ImagePool img_pool;
MusicPool music_pool;
AnimationPool anim_pool;
KeyState keystates[SDL_NUM_SCANCODES];
int anyKeyPressed;

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
    int destroyed, exploding;
} SnakePart;
typedef struct
{
    SnakePart parts[MAX_SNAKEPARTS];
    int length;
    Direction next_move_direction;
    double move_timer;
} Snake;

typedef struct
{
    int x, y;
    int activated, alert, disabled;
} Operator;

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
    int running, transitioning;
    GameState state, gotostate;
    int level;

    Snake snake;
    Food foods[MAX_FOODS];
    int foodcount;

    Board board;

    TextFont font;

    Image *boardImg;
    Image *snakeImg;
    Animation *snakeHeadNorthAnim;
    Animation *snakeHeadSouthAnim;
    Animation *snakeHeadWestAnim;
    Animation *snakeHeadEastAnim;

    Animation *snakeHeadWestNorthAnim;
    Animation *snakeHeadWestSouthAnim;
    Animation *snakeHeadEastNorthAnim;
    Animation *snakeHeadEastSouthAnim;
    Animation *snakeHeadNorthWestAnim;
    Animation *snakeHeadNorthEastAnim;
    Animation *snakeHeadSouthWestAnim;
    Animation *snakeHeadSouthEastAnim;

    Animation *snakePartExplodeAnim;

    Image *loseTextImg;
    Animation *loseAnim;

    // Image *transitionImg;
    // Animation *transitionAnim;

    Image *menuImg;
    Animation *menuCursorAnim;
    Animation *menuCursorWaitAnim;
    Operator menuCursor;

    Music *lowMus;
    Music *highMus;
} Game;
static Game game;

void moveOperator(Operator *operator, int xdelta, int ydelta)
{
    operator->x += xdelta;
    operator->y += ydelta;
}
void activateOperator(Operator *operator)
{
    if (!operator->disabled && operator->alert)
    {
        operator->activated = 1;
        operator->alert = 0;
    }
}
void deactivateOperator(Operator *operator)
{
    operator->activated = 0;
}
void disableOperator(Operator *operator)
{
    operator->disabled = 1;
    operator->alert = 0;
    operator->activated = 0;
}
void enableOperator(Operator *operator)
{
    operator->disabled = 0;
}
void alertOperator(Operator *operator)
{
    operator->alert = 1;
}
void settleOperator(Operator *operator)
{
    operator->alert = 0;
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
void setBoardGrass()
{
    int x, y;
    for (y = game.board.top; y < game.board.bottom; ++y)
    {
        for (x = game.board.left; x < game.board.right; ++x)
        {
            if (x == game.board.left || x == game.board.right - 1 || y == game.board.top || y == game.board.bottom - 1)
            {
                game.board.texture[x][y][0] = 3;
                game.board.texture[x][y][1] = 1;
            }
            else if (x == game.board.left + 1 && y == game.board.top + 1)
            {
                game.board.texture[x][y][0] = 0;
                game.board.texture[x][y][1] = 0;
            }
            else if (x == game.board.right - 2 && y == game.board.top + 1)
            {
                game.board.texture[x][y][0] = 2;
                game.board.texture[x][y][1] = 0;
            }
            else if (x == game.board.left + 1 && y == game.board.bottom - 2)
            {
                game.board.texture[x][y][0] = 0;
                game.board.texture[x][y][1] = 2;
            }
            else if (x == game.board.right - 2 && y == game.board.bottom - 2)
            {
                game.board.texture[x][y][0] = 2;
                game.board.texture[x][y][1] = 2;
            }
            else if (x == game.board.left + 1)
            {
                game.board.texture[x][y][0] = 0;
                game.board.texture[x][y][1] = 1;
            }
            else if (x == game.board.right - 2)
            {
                game.board.texture[x][y][0] = 2;
                game.board.texture[x][y][1] = 1;
            }
            else if (y == game.board.top + 1)
            {
                game.board.texture[x][y][0] = 1;
                game.board.texture[x][y][1] = 0;
            }
            else if (y == game.board.bottom - 2)
            {
                game.board.texture[x][y][0] = 1;
                game.board.texture[x][y][1] = 2;
            }
            else
            {
                game.board.texture[x][y][0] = 1;
                game.board.texture[x][y][1] = 1;
                if (rand() % 12 == 0)
                {
                    game.board.texture[x][y][0] = 3;
                    game.board.texture[x][y][1] = 0;
                }
            }
        }
    }
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
void gameMenuInit()
{
    game.menuImg = imageLoad("./img/mainmenumap.png");

    game.menuCursorAnim = animationCreate();
    addFrame(16, 2, 40000);
    addFrame(17, 2, 4000);
    addFrame(18, 2, 4000);
    addFrame(19, 2, 4000);
    addFrame(20, 2, 4000);
    animationFinish(1);

    game.menuCursorWaitAnim = animationCreate();
    addFrame(16, 3, 4000);
    addFrame(17, 3, 4000);
    addFrame(18, 3, 4000);
    addFrame(19, 3, 4000);
    addFrame(20, 3, 4000);
    animationFinish(1);

    game.menuCursor.x = screen_width / (2 * IMG_PIXEL_SIZE);
    game.menuCursor.y = screen_height / (2 * IMG_PIXEL_SIZE);
}
void gameMenuExit()
{
    imagePoolFreeAll();
    musicPoolFreeAll();
    animationPoolFreeAll();
}
void gameClassicInit()
{
    game.snakeImg = imageLoad("./img/snakemap.png");
    game.boardImg = imageLoad("./img/grassmap.png");

    game.lowMus = musicLoad("./snd/testsync.ogg");
    game.highMus = musicLoad("./snd/testsync2.ogg");

    musicPlay(game.lowMus);
    musicPlay(game.highMus);

    game.level = 0;
    int i;
    for (i = 0; i < game.snake.length; ++i)
    {
        game.snake.parts[i] = (SnakePart){};
    }
    game.snake = (Snake){};
    game.snake.length = 2;
    game.snake.parts[0].last_move_direction = EAST;
    game.snake.next_move_direction = EAST;
    game.snake.move_timer = MOVE_TIMER_RESET;
    putSnake(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);
    makeBoard(16, 16);
    setBoardGrass();

    game.snakeHeadNorthAnim = animationCreate();
    addFrame(0, 0, 8000);
    addFrame(1, 0, 8000);
    animationFinish(1);
    game.snakeHeadSouthAnim = animationCreate();
    addFrame(7, 1, 8000);
    addFrame(8, 1, 8000);
    animationFinish(1);
    game.snakeHeadWestAnim = animationCreate();
    addFrame(1, 3, 8000);
    addFrame(2, 3, 8000);
    animationFinish(1);
    game.snakeHeadEastAnim = animationCreate();
    addFrame(7, 0, 8000);
    addFrame(8, 0, 8000);
    animationFinish(1);

    game.snakeHeadWestNorthAnim = animationCreate();
    addFrame(9, 2, 8000);
    addFrame(9, 3, 8000);
    animationFinish(1);
    game.snakeHeadWestSouthAnim = animationCreate();
    addFrame(9, 0, 8000);
    addFrame(9, 1, 8000);
    animationFinish(1);
    game.snakeHeadEastNorthAnim = animationCreate();
    addFrame(4, 2, 8000);
    addFrame(5, 2, 8000);
    animationFinish(1);
    game.snakeHeadEastSouthAnim = animationCreate();
    addFrame(4, 1, 8000);
    addFrame(5, 1, 8000);
    animationFinish(1);
    game.snakeHeadNorthWestAnim = animationCreate();
    addFrame(1, 1, 8000);
    addFrame(2, 1, 8000);
    animationFinish(1);
    game.snakeHeadNorthEastAnim = animationCreate();
    addFrame(2, 0, 8000);
    addFrame(3, 0, 8000);
    animationFinish(1);
    game.snakeHeadSouthWestAnim = animationCreate();
    addFrame(10, 2, 8000);
    addFrame(10, 3, 8000);
    animationFinish(1);
    game.snakeHeadSouthEastAnim = animationCreate();
    addFrame(10, 0, 8000);
    addFrame(10, 1, 8000);
    animationFinish(1);

    game.snakePartExplodeAnim = animationCreate();
    addFrame(7, 2, 400);
    addFrame(8, 2, 400);
    addFrame(7, 3, 400);
    addFrame(8, 3, 400);
    animationFinish(1);

    spawnFood(16);

    fontLoad(&game.font, "./img/textmap.png", "0123456789:?!.,-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
}
void gameClassicExit()
{
    imagePoolFreeAll();
    musicPoolFreeAll();
    animationPoolFreeAll();
}
void gameLoseInit()
{
    game.loseTextImg = imageLoad("./img/losemap.png");

    game.loseAnim = animationCreate();
    addFrame(0, 0, 800);
    addFrame(0, 2, 800);
    addFrame(0, 4, 800);
    addFrame(0, 6, 800);
    addFrame(0, 8, 800);
    addFrame(0, 10, 800);
    addFrame(0, 12, 800);
    addFrame(0, 14, 800);
    addFrame(0, 16, 800);
    addFrame(0, 18, 800);
    addFrame(0, 20, 800);
    addFrame(0, 22, 800);
    addFrame(0, 24, 800);
    addFrame(0, 26, 800);
    addFrame(0, 28, 800);
    animationFinish(0);

    animationPause(game.loseAnim);
}

void gameGoTo(GameState state)
{
    game.gotostate = state;
}
void gameHandleGoTo()
{
    if (game.gotostate == game.state)
    {
        // game.transitioning = 0;
        return;
    }
    
    // if (!game.transitionImg)
    // {
    //     game.transitionImg = imageLoad("./img/transitionmap.png");
    // }
    // if (!game.transitionAnim)
    // {
    //     game.transitionAnim = animationCreate();
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     addFrame(0, 0, 800);
    //     animationFinish(0);
    // }
    // animationPoolUpdateAll();
    // game.transitioning = 1;
    // imageDrawAnimated(game.transitionImg, 0, 16 - game.transitionAnim->frame, game.transitionAnim);
    // if (game.transitionAnim->frame < game.transitionAnim->framecount - 1)
    // {
    //     printf("animation? %i\n", game.transitionAnim->frame);
    //     return;
    // }

    switch(game.state)
    {
        case GAME_MENU:
            gameMenuExit();
            break;
        case GAME_CLASSIC:
            if (game.gotostate != GAME_LOST)
                gameClassicExit();
            break;
        case GAME_LOST:
            gameClassicExit();
            break;
        default:
            break;
    }

    switch(game.gotostate)
    {
        case GAME_MENU:
            gameMenuInit();
            break;
        case GAME_CLASSIC:
            gameClassicInit();
            break;
        case GAME_LOST:
            gameLoseInit();
            break;
        default:
            break;
    }
    game.state = game.gotostate;
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
    int i;
    for (i = 0; i < game.snake.length; ++i)
    {
        if (game.snake.parts[i].destroyed || game.snake.parts[i].exploding)
            continue;

        int next_move_dir = getMoveDirection(game.snake.parts[i - 1].x - game.snake.parts[i].x,
                                            game.snake.parts[i - 1].y - game.snake.parts[i].y);
        // head
        if (i == 0)
        {
            switch(game.snake.parts[0].last_move_direction)
            {
            case NORTH:
                if (game.snake.next_move_direction == WEST)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadNorthWestAnim);
                }
                else if (game.snake.next_move_direction == EAST)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadNorthEastAnim);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadNorthAnim);
                }
                break;
            case SOUTH:
                if (game.snake.next_move_direction == WEST)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadSouthWestAnim);
                }
                else if (game.snake.next_move_direction == EAST)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadSouthEastAnim);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadSouthAnim);
                }
                break;
            case WEST:
                if (game.snake.next_move_direction == NORTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadWestNorthAnim);
                }
                else if (game.snake.next_move_direction == SOUTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadWestSouthAnim);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadWestAnim);
                }
                break;
            case EAST:
                if (game.snake.next_move_direction == NORTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadEastNorthAnim);
                }
                else if (game.snake.next_move_direction == SOUTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadEastSouthAnim);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadEastAnim);
                }
                break;
            default:
                break;
            }
        }
        // tail
        else if (i == game.snake.length - 1)
        {
            switch(next_move_dir)
            {
            case NORTH:
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 0, 3);
                break;
            case SOUTH:
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 1);
                break;
            case WEST:
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 5, 3);
                break;
            case EAST:
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 4, 0);
                break;
            default:
                break;
            }
        }
        // body
        else
        {
            switch(game.snake.parts[i].last_move_direction)
            {
            case NORTH:
                if (next_move_dir == WEST)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 1);
                }
                else if(next_move_dir == EAST)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 2);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 0, 1);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 0, 2);
                }
                break;
            case SOUTH:
                if (next_move_dir == WEST)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 1, 2);
                }
                else if(next_move_dir == EAST)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 2, 2);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 2);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 3);
                }
                break;
            case WEST:
                if (next_move_dir == NORTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 2, 2);
                }
                else if(next_move_dir == SOUTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 2);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 3);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 4, 3);
                }
                break;
            case EAST:
                if (next_move_dir == NORTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 1, 2);
                }
                else if(next_move_dir == SOUTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 1);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 5, 0);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 0);
                }
                break;
            default:
                break;
            }
        }
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

        imageDraw(game.boardImg, game.foods[i].x, game.foods[i].y, 3, 2);
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
            imageDraw(game.boardImg, x, y, game.board.texture[x][y][0], game.board.texture[x][y][1]);
        }
    }
}

void classicRules()
{
    int i;

    if (game.snake.parts[0].x == game.board.left ||
        game.snake.parts[0].x == game.board.right - 1 ||
        game.snake.parts[0].y == game.board.top ||
        game.snake.parts[0].y == game.board.bottom - 1)
    {
        gameGoTo(GAME_LOST);
    }

    for (i = 1; i < game.snake.length; ++i)
    {
        if (game.snake.parts[0].x == game.snake.parts[i].x &&
            game.snake.parts[0].y == game.snake.parts[i].y)
        {
            gameGoTo(GAME_LOST);
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

    if (game.snake.move_timer <= 0.0)
    {
        game.snake.move_timer = MOVE_TIMER_RESET;
        if (game.snake.parts[0].last_move_direction == getReverseDirection(game.snake.next_move_direction))
            game.snake.next_move_direction = game.snake.parts[0].last_move_direction;
        moveSnake(getXNormalFromDirection(game.snake.next_move_direction), getYNormalFromDirection(game.snake.next_move_direction));
    }
}
void menuRules()
{
    settleOperator(&game.menuCursor);

    if (game.menuCursor.x > 15)
        game.menuCursor.x = 15;
    if (game.menuCursor.x < 0)
        game.menuCursor.x = 0;
    if (game.menuCursor.y > 15)
        game.menuCursor.y = 15;
    if (game.menuCursor.y < 0)
        game.menuCursor.y = 0;

    if (game.menuCursor.x > 1 && game.menuCursor.x < 8 && game.menuCursor.y >= 8 && game.menuCursor.y <= 10)
    {
        alertOperator(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_PUZZLE);
    }
    if (game.menuCursor.x > 9 && game.menuCursor.x < 15 && game.menuCursor.y >= 8 && game.menuCursor.y <= 10)
    {
        alertOperator(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_CLASSIC);
    }
    if (game.menuCursor.x > 11 && game.menuCursor.y > 14)
    {
        alertOperator(&game.menuCursor);
        if (game.menuCursor.activated)
        {
            disableOperator(&game.menuCursor);
            game.running = false;
        }
    }
}
void drawMenuCursor()
{
    if (game.menuCursor.alert)
    {
        if (game.menuCursor.activated)
            imageDraw(game.menuImg, game.menuCursor.x, game.menuCursor.y, 17, 3);
        if (!game.menuCursor.activated)
            imageDraw(game.menuImg, game.menuCursor.x, game.menuCursor.y, 16, 3);
    }
    else if (game.menuCursor.disabled)
    {
        imageDrawAnimated(game.menuImg, game.menuCursor.x, game.menuCursor.y, game.menuCursorWaitAnim);
    }
    else
        imageDrawAnimated(game.menuImg, game.menuCursor.x, game.menuCursor.y, game.menuCursorAnim);
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

                anyKeyPressed++;
                if (keystates[event->key.keysym.scancode] == KEY_RELEASED)
                {
                    keystates[event->key.keysym.scancode] = KEY_TAPPED;
                    continue;
                }
                keystates[event->key.keysym.scancode] = KEY_HELD;
                break;
            case SDL_KEYUP:
                anyKeyPressed--;
                keystates[event->key.keysym.scancode] = KEY_RELEASED;
                break;
            default:
                break;
        }

    }
}
void handleGeneralInput()
{
    if (getKey(SDL_SCANCODE_ESCAPE) == KEY_TAPPED)
    {
        gameGoTo(GAME_MENU);
    }
    if (getKey(SDL_SCANCODE_F4) == KEY_TAPPED)
    {
        game.running = false;
    }
}
void handleLostInput()
{
    if (getAnyKey() && !getKey(SDL_SCANCODE_ESCAPE) && game.snake.parts[game.snake.length - 1].destroyed)
    {
        gameGoTo(GAME_CLASSIC);
    }
}
void handleClassicInput()
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
void handleMenuInput()
{
    if (getKey(SDL_SCANCODE_W) == KEY_TAPPED || getKey(SDL_SCANCODE_UP) == KEY_TAPPED)
    {
        moveOperator(&game.menuCursor, 0, -1);
    }
    if (getKey(SDL_SCANCODE_S) == KEY_TAPPED || getKey(SDL_SCANCODE_DOWN) == KEY_TAPPED)
    {
        moveOperator(&game.menuCursor, 0, 1);
    }
    if (getKey(SDL_SCANCODE_A) == KEY_TAPPED || getKey(SDL_SCANCODE_LEFT) == KEY_TAPPED)
    {
        moveOperator(&game.menuCursor, -1, 0);
    }
    if (getKey(SDL_SCANCODE_D) == KEY_TAPPED || getKey(SDL_SCANCODE_RIGHT) == KEY_TAPPED)
    {
        moveOperator(&game.menuCursor, 1, 0);
    }
    if (getKey(SDL_SCANCODE_SPACE) == KEY_TAPPED || getKey(SDL_SCANCODE_RETURN) == KEY_TAPPED || getKey(SDL_SCANCODE_Z) == KEY_TAPPED)
    {
        activateOperator(&game.menuCursor);
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

void snakeExplode()
{
    for (int i = 0; i < game.snake.length; ++i)
    {
        if (!game.snake.parts[i].destroyed)
        {
            game.snake.parts[i].exploding = 1;
            if (game.snakePartExplodeAnim->frame == game.snakePartExplodeAnim->framecount - 1)
            {
                game.snake.parts[i].destroyed = 1;
                game.snake.parts[i].exploding = 0;
                game.snakePartExplodeAnim->frame = 0;
            }
            imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakePartExplodeAnim);
            break;
        }
    }
}

static int mouseX, mouseY;
static SDL_Event event;
void gameLoop()
{
    SDL_RenderClear(app.renderer);

    updateKeys();
    sdlEventLoop(&event);
    handleGeneralInput();

    int game_caught_up = 0;
    gameUpdateTime();

    gameHandleGoTo();

    char scorebuf[128];

    switch(game.state)
    {
    case GAME_MENU:
        deactivateOperator(&game.menuCursor);
        handleMenuInput();
        while (!game_caught_up)
        {
            menuRules();
            animationPoolUpdateAll();

            game_caught_up = gameCaughtUp();
        }

        imageDrawLarge(game.menuImg, 0, 0, 0, 0, 16, 16);
        imageDrawLarge(game.menuImg, 12, 14, 20, 0, 4, 2);

        imageDrawLarge(game.menuImg, 2, 8, 24, 0, 5, 2);
        imageDrawLarge(game.menuImg, 10, 8, 24, 2, 5, 2);
        drawMenuCursor();
        break;
    case GAME_CLASSIC:
        handleClassicInput();
        while (!game_caught_up)
        {
            classicRules();
            updateFood();
            musicFadeIn(game.lowMus);
            musicFadeIn(game.highMus);
            animationPoolUpdateAll();

            game_caught_up = gameCaughtUp();
        }

        drawBoard();
        drawFood();
        drawSnake();
        sprintf(scorebuf, "%s%d", "Score: ", game.snake.length - 2);
        imageDrawText(scorebuf, &game.font, 0, 0);
        break;
    case GAME_LOST:
        handleLostInput();
        while (!game_caught_up)
        {
            musicFadeOut(game.lowMus);
            animationPoolUpdateAll();
            game_caught_up = gameCaughtUp();
        }
        drawBoard();
        drawFood();
        snakeExplode();
        drawSnake();
        if (game.snake.parts[game.snake.length - 1].destroyed)
        {
            animationUnpause(game.loseAnim);
            imageDrawLargeAnimated(game.loseTextImg, 0, 4, game.loseAnim, 16, 2);
            if (game.loseAnim->frame == game.loseAnim->framecount - 1)
            {
                imageDrawLarge(game.loseTextImg, 0, 6, 0, 30, 16, 2);
            }
        }
        imageDrawText(scorebuf, &game.font, game.loseAnim->frame / 3, (float)game.loseAnim->frame / 1.5f);
        break;
    default:
        // while (!game_caught_up)
        // {
        //     animationPoolUpdateAll();
        // }
        break;
    }

    SDL_RenderPresent(app.renderer);
}

int main(int argc, char **argv)
{
    srand(time(0));
    appInit();

    game.running = true;
    game.state = GAME_WON;
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