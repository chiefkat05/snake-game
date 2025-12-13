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

// no details plan:
    // unlock system
    // lose screen bug puzzle
    // makeboard for puzzles 1-3

// lose screen for puzzle is broken
// add three level selection images, and draw them based on how many levels are unlocked
// change selectRules to allow clicking on any unlocked level images (the unlocked part is not done)
// make food move over or down if it's on top of another food
// rename setboardgrass to setboardclassicgrass and make three more identical makeboard functions for makeboardpuzzle1-3
// add set obstacles to lvl1-3 boards
// add rules in lvl1-3 rules to treat the set obstacles like walls

// transition
// make transition animation and make sure it looks right by playing it at the start of one of the scenes
// idk go from there

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