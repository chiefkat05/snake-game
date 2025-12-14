#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "definitions.h"

#include "definitions.h"
#include "graphics.h"
#include "audio.h"
#include "application.h"
#include "animation.h"
#include "direction.h"
#include "keystate.h"
#include "text.h"
#include "screencursor.h"

// game plan

// go through all the graphics and update anything that isn't up to standard
// make the sfx and music for menu, select, classic, sandbox, and volcano

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

#define BOARD_WIDTH 16
#define BOARD_HEIGHT 16
typedef struct
{
    int texture[BOARD_WIDTH][BOARD_HEIGHT][2];
    int wall[BOARD_WIDTH][BOARD_HEIGHT];
    int width, height;

    int left, right, top, bottom;
} Board;

typedef enum
{
    GAME_MENU,
    GAME_SELECT,
    GAME_PUZZLE1,
    GAME_PUZZLE2,
    GAME_PUZZLE3,
    GAME_PUZZLE4,
    GAME_PUZZLE5,
    GAME_PUZZLE6,
    GAME_PUZZLE7,
    GAME_PUZZLE8,
    GAME_PUZZLE9,
    GAME_PUZZLE10,
    GAME_PUZZLE11,
    GAME_PUZZLE12,
    GAME_SETTINGS,
    GAME_CLASSIC,
    GAME_LOST_CLASSIC,
    GAME_LOST_PUZZLE,
    GAME_WON
} GameState;

typedef enum
{
    BOARD_GRASS,
    BOARD_SANDBOX,
    BOARD_VOLCANO
} BoardType;

typedef struct
{
    int running, transitioning;
    GameState state, gotostate;
    int current_level, unlocked_levelcount;

    Snake snake;
    Food foods[MAX_FOODS];
    int foodcount;

    Board board;

    TextFont font;

    Image *grassBoardImg;
    Image *sandBoardImg;
    Image *volcanoBoardImg;
    Animation *volcanoWallAnim;
    Animation *volcanoWall2Anim;

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
    Image *winTextImg;
    Animation *winAnim;

    Image *menuImg;
    Animation *menuCursorAnim;
    Animation *menuCursorWaitAnim;
    ScreenCursor menuCursor;

    Image *selectImg;
    Animation *selectBGAnim;

    Music *lowMus;
    Music *highMus;
} Game;
extern Game game;

void makeBoard(int width, int height);
void setBoardGrass();
void spawnFood(int foodnum);
void putFood(int x, int y);
void gameMenuInit();
void gameSelectInit();
void gameClassicInit();
void gamePuzzleInit();
void gameSceneExit();
void gameLoseInit();

void gameGoTo(GameState state);
void gameHandleGoTo();

void gameUpdateTime();
int gameCaughtUp();

void putSnake(int x, int y);
void moveSnake(int xdelta, int ydelta);
void drawSnake();

void updateClassicFood();
void drawFood();
void drawBoard();

void classicRules();
void puzzleRules();
void menuRules();
void selectRules();
void drawMenuCursor();

void sdlEventLoop(SDL_Event *event);
void handleGeneralInput();
void handleLostClassicInput();
void handleLostPuzzleInput();
void handleWonInput();
void handleClassicInput();
void handlePuzzleInput();
void handleMenuInput();

void snakeExplode();

void gameLoop();

#endif