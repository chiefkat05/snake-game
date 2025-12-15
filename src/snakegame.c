#include "snakegame.h"

Application app;
ImagePool img_pool;
MusicPool music_pool;
AnimationPool anim_pool;
KeyState keystates[SDL_NUM_SCANCODES];
int anyKeyPressed;
static int mouseX, mouseY;
static SDL_Event event;

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
void setBoard(BoardType type)
{
    int x, y;
    if (type == BOARD_GRASS)
    {
    for (y = game.board.top; y < game.board.bottom; ++y)
    {
        for (x = game.board.left; x < game.board.right; ++x)
        {
            game.board.wall[x][y] = 0;
            if (x == game.board.left || x == game.board.right - 1 || y == game.board.top || y == game.board.bottom - 1)
            {
                game.board.texture[x][y][0] = 3;
                game.board.texture[x][y][1] = 1;
                game.board.wall[x][y] = 1;
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

    if (type == BOARD_SANDBOX)
    {
    for (y = game.board.top; y < game.board.bottom; ++y)
    {
        for (x = game.board.left; x < game.board.right; ++x)
        {
            game.board.wall[x][y] = 0;
            if (x == game.board.left && y == game.board.top ||
                x == game.board.right - 1 && y == game.board.top)
            {
                game.board.texture[x][y][0] = 0;
                game.board.texture[x][y][1] = 1;
                game.board.wall[x][y] = 1;
            }
            else if (x == game.board.left && y == game.board.bottom - 1 ||
                x == game.board.right - 1 && y == game.board.bottom - 1)
            {
                game.board.texture[x][y][0] = 0;
                game.board.texture[x][y][1] = 2;
                game.board.wall[x][y] = 1;
            }
            else if (x == game.board.left)
            {
                game.board.texture[x][y][0] = 0;
                game.board.texture[x][y][1] = 0;
                game.board.wall[x][y] = 1;
            }
            else if (x == game.board.right - 1)
            {
                game.board.texture[x][y][0] = 3;
                game.board.texture[x][y][1] = 0;
                game.board.wall[x][y] = 1;
            }
            else if (y == game.board.top)
            {
                game.board.texture[x][y][0] = 2;
                game.board.texture[x][y][1] = 0;
                game.board.wall[x][y] = 1;
            }
            else if (y == game.board.bottom - 1)
            {
                game.board.texture[x][y][0] = 4;
                game.board.texture[x][y][1] = 0;
                game.board.wall[x][y] = 1;
            }
            else
            {
                game.board.texture[x][y][0] = 1;
                game.board.texture[x][y][1] = 0;
            }
        }
    }
    }

    if (type == BOARD_VOLCANO)
    {
    for (y = game.board.top; y < game.board.bottom; ++y)
    {
        for (x = game.board.left; x < game.board.right; ++x)
        {
            game.board.wall[x][y] = 0;
            if (x == game.board.left || x == game.board.right - 1 || y == game.board.top || y == game.board.bottom - 1)
            {
                int walltype = rand() % 24;
                if (walltype == 0)
                {
                    game.board.texture[x][y][0] = 0;
                    game.board.texture[x][y][1] = 0;
                }
                else if (walltype == 1)
                {
                    game.board.texture[x][y][0] = 0;
                    game.board.texture[x][y][1] = 1;
                }
                else
                {
                    game.board.texture[x][y][0] = 1;
                    game.board.texture[x][y][1] = 2;
                }
                game.board.wall[x][y] = 1;
            }
            else
            {
                game.board.texture[x][y][0] = 2;
                game.board.texture[x][y][1] = 2;
            }
        }
    }
    }
}
void setBoardWall(int x, int y, BoardType type)
{
    switch(type)
    {
        case BOARD_GRASS:
            game.board.texture[game.board.left + x][game.board.top + y][0] = 3;
            game.board.texture[game.board.left + x][game.board.top + y][1] = 1;
            break;
        case BOARD_SANDBOX:
            game.board.texture[game.board.left + x][game.board.top + y][0] = 0;
            game.board.texture[game.board.left + x][game.board.top + y][1] = 2;
            break;
        case BOARD_VOLCANO:
            int walltype = rand() % 24;
            if (walltype == 0)
            {
                game.board.texture[game.board.left + x][game.board.top + y][0] = 0;
                game.board.texture[game.board.left + x][game.board.top + y][1] = 0;
            }
            else if (walltype == 1)
            {
                game.board.texture[game.board.left + x][game.board.top + y][0] = 0;
                game.board.texture[game.board.left + x][game.board.top + y][1] = 1;
            }
            else
            {
                game.board.texture[game.board.left + x][game.board.top + y][0] = 1;
                game.board.texture[game.board.left + x][game.board.top + y][1] = 2;
            }
            break;
        default:
            break;
    }
    game.board.wall[game.board.left + x][game.board.top + y] = 1;
}
void setBoardCustomWall(int x, int y, int tx, int ty)
{
    game.board.texture[game.board.left + x][game.board.top + y][0] = tx;
    game.board.texture[game.board.left + x][game.board.top + y][1] = ty;
    game.board.wall[game.board.left + x][game.board.top + y] = 1;
}
void setBoardNothing(int x, int y)
{
    game.board.wall[game.board.left + x][game.board.top + y] = -1;
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
void putFood(int x, int y)
{
    x += game.board.left;
    y += game.board.top;
    game.foods[game.foodcount].x = x;
    game.foods[game.foodcount].y = y;
    ++game.foodcount;
}

void musicHandler()
{
    if (game.gotostate != game.state)
    {
        musicStop();
    }

    if (Mix_Playing(-1) == 0)
        return;

        switch(game.state)
        {
            case GAME_MENU:
                musicFadeIn(game.menuMus, __LINE__);
                break;
            case GAME_CLASSIC:
                musicFadeIn(game.grassMus, __LINE__);
                break;
            case GAME_PUZZLE1:
            case GAME_PUZZLE2:
            case GAME_PUZZLE3:
            case GAME_PUZZLE4:
                musicFadeIn(game.grassMus, __LINE__);
                break;
            case GAME_PUZZLE5:
            case GAME_PUZZLE6:
            case GAME_PUZZLE7:
            case GAME_PUZZLE8:
                musicFadeIn(game.sandboxMus, __LINE__);
                break;
            case GAME_PUZZLE9:
            case GAME_PUZZLE10:
            case GAME_PUZZLE11:
            case GAME_PUZZLE12:
                musicFadeIn(game.volcanoMus, __LINE__);
                break;
            case GAME_SELECT:
                musicFadeIn(game.selectMus, __LINE__);
                break;
            case GAME_WON:
                musicFadeIn(game.winMus, __LINE__);
                break;
            case GAME_LOST_PUZZLE:
            case GAME_LOST_CLASSIC:
                musicFadeIn(game.winMus, __LINE__);
                break;
            default:
                break;
        }
}

void gameMenuInit()
{
    game.menuImg = imageLoad("./img/mainmenumap.png");

    if (game.gotostate == GAME_MENU)
        musicPlay(game.menuMus, 1, 1, __LINE__);

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

    game.menuSnakeBlinkAnim = animationCreate();
    addFrame(16, 6, 80000);
    addFrame(18, 6, 2000);
    addFrame(16, 6, 60000);
    addFrame(18, 6, 2000);
    addFrame(16, 8, 5000);
    addFrame(18, 6, 2000);
    animationFinish(1);
    game.menuTitleShineAnim = animationCreate();
    addFrame(32, 0, 90000);
    addFrame(20, 5, 1000);
    addFrame(20, 10, 1000);
    addFrame(32, 5, 1000);
    addFrame(32, 10, 1000);
    animationFinish(1);

    game.menuCursor.x = screen_width / (2 * IMG_PIXEL_SIZE);
    game.menuCursor.y = screen_height / (2 * IMG_PIXEL_SIZE);
}
void gameSelectInit()
{
    game.selectImg = imageLoad("./img/selectmap.png");

    musicPlay(game.selectMus, 1, 1, __LINE__);

    game.selectBGAnim = animationCreate();
    addFrame(0, 0, 8000);
    addFrame(8, 0, 8000);
    addFrame(0, 8, 8000);
    addFrame(8, 8, 8000);
    animationFinish(1);
}
void baseGameInit()
{
    game.snakeImg = imageLoad("./img/snakemap.png");
    game.grassBoardImg = imageLoad("./img/grassmap.png");

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
    game.foodcount = 0;
    for (i = 0; i < MAX_FOODS; ++i)
    {
        game.foods[i].eaten = 0;
    }

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

    fontLoad(&game.font, "./img/textmap.png", "0123456789:?!.,-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/ ");
}
void gameClassicInit()
{
    baseGameInit();

    musicPlay(game.grassMus, 1, 0, __LINE__);

    makeBoard(16, 16);
    setBoard(BOARD_GRASS);
    spawnFood(16);
}
void gamePuzzleInit()
{
    baseGameInit();

    if (game.current_level < 4)
    {
        musicPlay(game.grassMus, 1, 0, __LINE__);
    }
    else if (game.current_level < 8)
    {
        musicPlay(game.sandboxMus, 1, 0, __LINE__);
    }
    else if (game.current_level < 12)
    {
        musicPlay(game.volcanoMus, 1, 0, __LINE__);
    }

    game.winTextImg = imageLoad("./img/winmap.png");
    game.winAnim = animationCreate();
    addFrame(0, 0, 800);
    addFrame(0, 2, 800);
    addFrame(0, 4, 800);
    addFrame(0, 6, 800);
    addFrame(0, 8, 800);
    addFrame(0, 10, 800);
    addFrame(0, 12, 800);
    animationFinish(0);
    animationPause(game.winAnim);

    game.sandBoardImg = imageLoad("./img/sandboxmap.png");
    game.volcanoBoardImg = imageLoad("./img/volcanomap.png");
    game.volcanoWallAnim = animationCreate();
    addFrame(0, 0, 4000);
    addFrame(1, 0, 4000);
    addFrame(2, 0, 4000);
    animationFinish(1);
    game.volcanoWall2Anim = animationCreate();
    addFrame(0, 1, 4000);
    addFrame(1, 1, 4000);
    addFrame(2, 1, 4000);
    animationFinish(1);

    switch(game.current_level)
    {
        case 0:
            makeBoard(5, 5);
            setBoard(BOARD_GRASS);
            putFood(1, 2);
            putFood(2, 1);
            break;
        case 1:
            makeBoard(6, 6);
            setBoard(BOARD_GRASS);
            putFood(2, 2);
            putFood(3, 2);
            putFood(2, 4);
            putFood(4, 3);
            setBoardWall(4, 1, BOARD_GRASS);
            setBoardWall(4, 4, BOARD_GRASS);
            break;
        case 2:
            makeBoard(7, 8);
            setBoard(BOARD_GRASS);
            setBoardWall(2, 2, BOARD_GRASS);
            setBoardWall(2, 3, BOARD_GRASS);
            setBoardWall(2, 4, BOARD_GRASS);
            setBoardWall(2, 5, BOARD_GRASS);
            putFood(1, 2);
            putFood(1, 3);
            putFood(1, 4);
            putFood(1, 5);
            putFood(4, 1);
            putFood(4, 6);
            break;
        case 3:
            makeBoard(16, 8);
            setBoard(BOARD_GRASS);
            putSnake(2, 7);
            setBoardWall(2, 4, BOARD_GRASS);
            setBoardWall(3, 4, BOARD_GRASS);
            setBoardWall(3, 3, BOARD_GRASS);
            setBoardWall(3, 2, BOARD_GRASS);
            setBoardWall(3, 1, BOARD_GRASS);
            setBoardWall(5, 3, BOARD_GRASS);
            setBoardWall(6, 3, BOARD_GRASS);
            setBoardWall(7, 3, BOARD_GRASS);
            setBoardWall(8, 3, BOARD_GRASS);
            setBoardWall(8, 4, BOARD_GRASS);
            setBoardWall(8, 5, BOARD_GRASS);
            setBoardWall(8, 6, BOARD_GRASS);
            setBoardWall(5, 2, BOARD_GRASS);
            setBoardWall(7, 1, BOARD_GRASS);
            setBoardWall(11, 4, BOARD_GRASS);
            setBoardWall(12, 5, BOARD_GRASS);
            putFood(2, 2);
            putFood(1, 4);
            putFood(13, 5);
            putFood(12, 4);
            putFood(11, 3);
            putFood(7, 2);
            putFood(5, 1);
            putFood(7, 4);
            putFood(7, 5);
            break;
        case 4:
            makeBoard(5, 5);
            setBoard(BOARD_SANDBOX);
            setBoardNothing(4, 0);
            setBoardNothing(0, 0);
            setBoardCustomWall(3, 0, 0, 1);
            setBoardCustomWall(1, 0, 0, 1);
            setBoardWall(3, 1, BOARD_SANDBOX);
            setBoardWall(4, 1, BOARD_SANDBOX);
            setBoardWall(0, 1, BOARD_SANDBOX);
            setBoardWall(1, 1, BOARD_SANDBOX);
            putFood(1, 2);
            putFood(2, 1);
            break;
        case 5:
            makeBoard(10, 7);
            setBoard(BOARD_SANDBOX);
            putSnake(6, 5);
            setBoardWall(3, 2, BOARD_SANDBOX);
            setBoardCustomWall(4, 2, 4, 0);
            setBoardCustomWall(5, 2, 4, 0);
            setBoardCustomWall(6, 2, 4, 0);
            setBoardWall(7, 2, BOARD_SANDBOX);
            setBoardCustomWall(7, 3, 0, 0);
            setBoardWall(7, 4, BOARD_SANDBOX);
            setBoardCustomWall(6, 4, 2, 0);
            setBoardCustomWall(5, 4, 2, 0);
            setBoardCustomWall(4, 4, 2, 0);
            setBoardWall(3, 4, BOARD_SANDBOX);
            setBoardCustomWall(3, 3, 3, 0);
            setBoardNothing(4, 3);
            setBoardNothing(5, 3);
            setBoardNothing(6, 3);
            putFood(2, 2);
            putFood(1, 2);
            putFood(1, 3);
            putFood(2, 3);
            putFood(1, 4);
            putFood(2, 4);
            break;
        case 6:
            makeBoard(11, 11);
            setBoard(BOARD_SANDBOX);
            putSnake(6, 10);
            setBoardCustomWall(4, 3, 5, 0);
            setBoardWall(3, 4, BOARD_SANDBOX);
            setBoardCustomWall(3, 3, 0, 1);
            setBoardCustomWall(3, 6, 0, 1);
            setBoardWall(3, 7, BOARD_SANDBOX);
            setBoardCustomWall(5, 8, 0, 1);
            setBoardCustomWall(5, 9, 6, 0);
            setBoardWall(2, 4, BOARD_SANDBOX);
            setBoardWall(5, 3, BOARD_SANDBOX);
            setBoardCustomWall(6, 3, 5, 0);
            setBoardCustomWall(7, 3, 5, 0);
            setBoardCustomWall(8, 3, 5, 0);
            setBoardWall(9, 3, BOARD_SANDBOX);
            setBoardWall(5, 5, BOARD_SANDBOX);
            setBoardCustomWall(6, 5, 5, 0);
            setBoardCustomWall(7, 5, 0, 1);
            setBoardWall(7, 7, BOARD_SANDBOX);
            setBoardCustomWall(7, 6, 6, 0);
            setBoardCustomWall(8, 7, 5, 0);
            setBoardCustomWall(9, 7, 5, 0);
            setBoardCustomWall(1, 1, 2, 0);
            setBoardCustomWall(2, 1, 2, 0);
            setBoardWall(3, 1, BOARD_SANDBOX);
            setBoardCustomWall(5, 2, 0, 1);
            setBoardWall(7, 1, BOARD_SANDBOX);
            setBoardCustomWall(9, 2, 0, 1);
            setBoardCustomWall(0, 1, 0, 1);
            setBoardCustomWall(3, 0, 0, 1);
            setBoardCustomWall(7, 0, 0, 1);
            setBoardWall(5, 10, BOARD_SANDBOX);
            setBoardWall(10, 7, BOARD_SANDBOX);
            setBoardNothing(0, 0);
            setBoardNothing(1, 0);
            setBoardNothing(2, 0);
            setBoardCustomWall(10, 2, 0, 1);
            putFood(4, 7);
            putFood(4, 6);
            putFood(4, 5);
            putFood(4, 4);
            putFood(1, 9);
            putFood(9, 1);
            putFood(9, 9);
            break;
        case 7:
            makeBoard(8, 16);
            setBoard(BOARD_SANDBOX);
            setBoardCustomWall(0, 13, 0, 1);
            setBoardCustomWall(1, 13, 5, 0);
            setBoardCustomWall(2, 13, 5, 0);
            setBoardCustomWall(3, 13, 5, 0);
            setBoardCustomWall(4, 13, 5, 0);
            setBoardWall(5, 13, BOARD_SANDBOX);
            
            setBoardCustomWall(7, 11, 0, 1);
            setBoardCustomWall(6, 11, 5, 0);
            setBoardCustomWall(5, 11, 5, 0);
            setBoardCustomWall(4, 11, 5, 0);
            setBoardCustomWall(3, 11, 5, 0);

            setBoardWall(2, 11, BOARD_SANDBOX);
            setBoardCustomWall(2, 10, 6, 0);
            setBoardCustomWall(2, 9, 6, 0);
            setBoardCustomWall(2, 8, 0, 1);

            setBoardCustomWall(0, 6, 0, 1);
            setBoardCustomWall(1, 6, 5, 0);
            setBoardCustomWall(2, 6, 5, 0);
            setBoardCustomWall(3, 6, 5, 0);
            
            setBoardCustomWall(4, 6, 0, 1);
            setBoardCustomWall(4, 7, 6, 0);
            setBoardCustomWall(4, 8, 6, 0);

            setBoardWall(4, 9, BOARD_SANDBOX);
            setBoardWall(5, 9, BOARD_SANDBOX);

            setBoardCustomWall(7, 7, 0, 1);
            setBoardWall(6, 7, BOARD_SANDBOX);
            setBoardCustomWall(6, 6, 3, 0);
            setBoardCustomWall(6, 5, 3, 0);
            setBoardCustomWall(6, 4, 0, 1);
            setBoardWall(7, 4, BOARD_SANDBOX);
            setBoardNothing(7, 5);
            setBoardNothing(7, 6);

            setBoardCustomWall(0, 2, 0, 1);
            setBoardCustomWall(1, 2, 5, 0);
            setBoardCustomWall(2, 2, 5, 0);
            setBoardCustomWall(3, 2, 5, 0);
            setBoardCustomWall(4, 2, 5, 0);
            setBoardWall(5, 2, BOARD_SANDBOX);

            setBoardWall(2, 4, BOARD_SANDBOX);
            setBoardCustomWall(3, 4, 5, 0);
            setBoardCustomWall(4, 4, 5, 0);
            setBoardCustomWall(5, 4, 5, 0);

            game.snake.length = 4;
            putSnake(7, 14);
            moveSnake(1, 0);
            putFood(1, 1);
            break;

        case 8:
            makeBoard(16, 8);
            setBoard(BOARD_VOLCANO);
            setBoardNothing(0, 0);
            setBoardNothing(1, 0);
            setBoardNothing(2, 0);
            setBoardNothing(3, 0);
            setBoardNothing(4, 0);
            setBoardNothing(5, 0);
            setBoardNothing(6, 0);
            setBoardNothing(7, 0);
            setBoardNothing(8, 0);
            setBoardNothing(9, 0);
            setBoardNothing(10, 0);
            setBoardNothing(11, 0);
            setBoardNothing(0, 1);
            setBoardNothing(1, 1);
            setBoardNothing(2, 1);
            setBoardNothing(3, 1);
            setBoardNothing(4, 1);
            setBoardNothing(5, 1);
            setBoardNothing(6, 1);
            setBoardNothing(7, 1);
            setBoardNothing(8, 1);
            setBoardNothing(9, 1);
            setBoardNothing(10, 1);
            setBoardNothing(11, 1);
            setBoardNothing(0, 2);
            setBoardNothing(1, 2);
            setBoardNothing(2, 2);
            setBoardNothing(3, 2);
            setBoardNothing(4, 2);
            setBoardNothing(5, 2);
            setBoardNothing(6, 2);
            setBoardNothing(7, 2);
            setBoardNothing(8, 2);
            setBoardNothing(9, 2);
            setBoardNothing(10, 2);
            setBoardNothing(11, 2);
            setBoardNothing(0, 3);
            setBoardNothing(1, 3);
            setBoardNothing(2, 3);
            setBoardNothing(3, 3);
            setBoardNothing(4, 3);
            setBoardNothing(5, 3);
            setBoardNothing(6, 3);
            setBoardNothing(7, 3);
            setBoardNothing(8, 3);
            setBoardNothing(9, 3);
            setBoardNothing(10, 3);
            setBoardNothing(11, 3);

            setBoardWall(1, 4, BOARD_VOLCANO);
            setBoardWall(2, 4, BOARD_VOLCANO);
            setBoardWall(3, 4, BOARD_VOLCANO);
            setBoardWall(4, 4, BOARD_VOLCANO);
            setBoardWall(5, 4, BOARD_VOLCANO);
            setBoardWall(6, 4, BOARD_VOLCANO);
            setBoardWall(7, 4, BOARD_VOLCANO);
            setBoardWall(8, 4, BOARD_VOLCANO);
            setBoardWall(9, 4, BOARD_VOLCANO);
            setBoardWall(10, 4, BOARD_VOLCANO);
            setBoardWall(11, 4, BOARD_VOLCANO);
            setBoardWall(12, 4, BOARD_VOLCANO);
            setBoardWall(12, 3, BOARD_VOLCANO);
            setBoardWall(12, 2, BOARD_VOLCANO);
            setBoardWall(12, 1, BOARD_VOLCANO);
            putSnake(2, 9);
            putFood(14, 2);
            putFood(13, 1);
            break;
        case 9:
            makeBoard(12, 16);
            setBoard(BOARD_VOLCANO);
            setBoardNothing(0, 1);
            setBoardNothing(1, 1);
            setBoardNothing(2, 1);
            setBoardNothing(3, 1);
            setBoardNothing(4, 1);
            setBoardNothing(5, 1);

            setBoardNothing(0, 0);
            setBoardNothing(1, 0);
            setBoardNothing(2, 0);
            setBoardNothing(3, 0);
            setBoardNothing(4, 0);
            setBoardNothing(5, 0);


            setBoardNothing(0, 4);
            setBoardNothing(5, 5);
            setBoardNothing(4, 5);
            setBoardNothing(4, 6);
            setBoardNothing(4, 7);
            setBoardNothing(5, 7);
            setBoardNothing(6, 7);
            setBoardNothing(7, 7);
            setBoardNothing(7, 8);
            setBoardNothing(7, 9);
            setBoardNothing(7, 10);
            setBoardNothing(8, 10);
            setBoardNothing(9, 10);
            setBoardNothing(10, 10);
            setBoardNothing(11, 10);
            setBoardNothing(11, 9);
            setBoardNothing(11, 8);
            setBoardNothing(11, 7);
            setBoardNothing(11, 6);
            setBoardNothing(11, 5);
            setBoardNothing(11, 4);
            setBoardNothing(0, 2);
            setBoardNothing(0, 3);

            setBoardWall(3, 13, BOARD_VOLCANO);
            setBoardWall(3, 14, BOARD_VOLCANO);
            setBoardWall(4, 13, BOARD_VOLCANO);
            setBoardWall(5, 13, BOARD_VOLCANO);
            setBoardWall(6, 13, BOARD_VOLCANO);
            setBoardWall(7, 13, BOARD_VOLCANO);
            setBoardWall(8, 13, BOARD_VOLCANO);
            setBoardWall(8, 14, BOARD_VOLCANO);

            setBoardNothing(4, 14);
            setBoardNothing(5, 14);
            setBoardNothing(6, 14);
            setBoardNothing(7, 14);

            setBoardNothing(4, 15);
            setBoardNothing(5, 15);
            setBoardNothing(6, 15);
            setBoardNothing(7, 15);

            putSnake(10, 2);
            putFood(9, 8);
            putFood(10, 1);
            putFood(9, 1);
            putFood(10, 2);
            putFood(1, 13);
            putFood(2, 13);
            putFood(9, 13);
            putFood(10, 13);
            putFood(5, 9);

            setBoardWall(8, 9, BOARD_VOLCANO);
            setBoardWall(9, 9, BOARD_VOLCANO);
            setBoardWall(10, 9, BOARD_VOLCANO);

            setBoardWall(10, 11, BOARD_VOLCANO);
            setBoardWall(9, 11, BOARD_VOLCANO);
            setBoardWall(8, 11, BOARD_VOLCANO);
            setBoardWall(7, 11, BOARD_VOLCANO);
            setBoardWall(6, 11, BOARD_VOLCANO);

            setBoardWall(6, 10, BOARD_VOLCANO);
            setBoardWall(6, 9, BOARD_VOLCANO);
            setBoardWall(6, 8, BOARD_VOLCANO);
            setBoardWall(5, 8, BOARD_VOLCANO);
            setBoardWall(4, 8, BOARD_VOLCANO);

            setBoardWall(8, 8, BOARD_VOLCANO);
            setBoardWall(8, 7, BOARD_VOLCANO);
            setBoardWall(8, 6, BOARD_VOLCANO);
            
            setBoardWall(10, 8, BOARD_VOLCANO);
            setBoardWall(10, 7, BOARD_VOLCANO);
            setBoardWall(10, 6, BOARD_VOLCANO);
            setBoardWall(10, 5, BOARD_VOLCANO);
            setBoardWall(10, 4, BOARD_VOLCANO);
            setBoardWall(10, 3, BOARD_VOLCANO);

            setBoardWall(6, 1, BOARD_VOLCANO);
            setBoardWall(6, 2, BOARD_VOLCANO);
            setBoardWall(5, 2, BOARD_VOLCANO);
            setBoardWall(4, 2, BOARD_VOLCANO);
            setBoardWall(3, 2, BOARD_VOLCANO);
            setBoardWall(2, 2, BOARD_VOLCANO);
            setBoardWall(1, 2, BOARD_VOLCANO);
            setBoardWall(1, 3, BOARD_VOLCANO);
            setBoardWall(1, 4, BOARD_VOLCANO);
            setBoardWall(1, 5, BOARD_VOLCANO);

            setBoardWall(3, 4, BOARD_VOLCANO);
            setBoardWall(4, 4, BOARD_VOLCANO);
            setBoardWall(5, 4, BOARD_VOLCANO);
            setBoardWall(6, 4, BOARD_VOLCANO);

            setBoardWall(6, 5, BOARD_VOLCANO);
            setBoardWall(6, 6, BOARD_VOLCANO);
            setBoardWall(5, 6, BOARD_VOLCANO);
            setBoardWall(7, 6, BOARD_VOLCANO);


            setBoardWall(3, 5, BOARD_VOLCANO);
            setBoardWall(3, 6, BOARD_VOLCANO);
            setBoardWall(3, 7, BOARD_VOLCANO);
            setBoardWall(3, 8, BOARD_VOLCANO);
            break;
        case 10:
            makeBoard(12, 12);
            setBoard(BOARD_VOLCANO);
            putFood(1, 1);
            putFood(1, 10);
            putFood(10, 10);
            putFood(10, 1);
            putFood(8, 3);
            putFood(8, 8);
            putFood(3, 8);
            putFood(6, 5);
            putFood(3, 5);

            setBoardWall(2, 2, BOARD_VOLCANO);
            setBoardWall(3, 2, BOARD_VOLCANO);
            setBoardWall(4, 2, BOARD_VOLCANO);
            setBoardWall(5, 2, BOARD_VOLCANO);
            setBoardWall(6, 2, BOARD_VOLCANO);
            setBoardWall(7, 2, BOARD_VOLCANO);
            setBoardWall(8, 2, BOARD_VOLCANO);

            setBoardWall(9, 2, BOARD_VOLCANO);
            setBoardWall(9, 3, BOARD_VOLCANO);
            setBoardWall(9, 4, BOARD_VOLCANO);
            setBoardWall(9, 5, BOARD_VOLCANO);
            setBoardWall(9, 6, BOARD_VOLCANO);
            setBoardWall(9, 7, BOARD_VOLCANO);
            setBoardWall(9, 8, BOARD_VOLCANO);

            setBoardWall(9, 9, BOARD_VOLCANO);
            setBoardWall(8, 9, BOARD_VOLCANO);
            setBoardWall(7, 9, BOARD_VOLCANO);
            setBoardWall(6, 9, BOARD_VOLCANO);
            setBoardWall(5, 9, BOARD_VOLCANO);
            setBoardWall(4, 9, BOARD_VOLCANO);
            setBoardWall(3, 9, BOARD_VOLCANO);

            setBoardWall(2, 9, BOARD_VOLCANO);
            setBoardWall(2, 8, BOARD_VOLCANO);
            setBoardWall(2, 7, BOARD_VOLCANO);
            setBoardWall(2, 6, BOARD_VOLCANO);
            setBoardWall(2, 5, BOARD_VOLCANO);

            setBoardWall(2, 4, BOARD_VOLCANO);
            setBoardWall(3, 4, BOARD_VOLCANO);
            setBoardWall(4, 4, BOARD_VOLCANO);
            setBoardWall(5, 4, BOARD_VOLCANO);
            setBoardWall(6, 4, BOARD_VOLCANO);

            setBoardWall(7, 4, BOARD_VOLCANO);
            setBoardWall(7, 5, BOARD_VOLCANO);
            setBoardWall(7, 6, BOARD_VOLCANO);

            setBoardWall(7, 7, BOARD_VOLCANO);
            setBoardWall(6, 7, BOARD_VOLCANO);
            setBoardWall(5, 7, BOARD_VOLCANO);
            setBoardWall(4, 7, BOARD_VOLCANO);

            game.snake.length = 3;
            putSnake(7, 8);
            moveSnake(1, 0);
            break;
        case 11:
            makeBoard(16, 16);
            setBoard(BOARD_VOLCANO);
            putFood(2, 5);
            putFood(2, 7);
            putFood(14, 12);
            putFood(14, 11);
            putFood(9, 11);
            putFood(14, 2);
            putFood(14, 3);
            putFood(1, 14);
            putFood(1, 3);

            setBoardNothing(15, 15);
            setBoardNothing(14, 15);
            setBoardNothing(14, 14);
            setBoardNothing(15, 14);
            setBoardNothing(13, 14);
            setBoardNothing(13, 15);

            setBoardNothing(6, 0);
            setBoardNothing(7, 0);
            setBoardNothing(8, 0);
            setBoardNothing(9, 0);
            setBoardNothing(10, 0);
            setBoardNothing(11, 0);
            setBoardNothing(12, 0);
            setBoardNothing(13, 0);
            setBoardNothing(14, 0);
            setBoardNothing(15, 0);

            setBoardNothing(0, 5);
            setBoardNothing(0, 6);
            setBoardNothing(0, 7);
            setBoardNothing(0, 8);

            setBoardNothing(4, 5);
            setBoardNothing(4, 6);
            setBoardNothing(4, 7);
            setBoardNothing(4, 8);

            setBoardNothing(8, 9);
            setBoardNothing(8, 8);
            setBoardNothing(8, 7);
            setBoardNothing(8, 6);
            setBoardNothing(8, 5);
            setBoardWall(7, 4, BOARD_VOLCANO);
            setBoardWall(8, 4, BOARD_VOLCANO);
            setBoardWall(9, 4, BOARD_VOLCANO);
            setBoardWall(11, 4, BOARD_VOLCANO);

            setBoardWall(12, 4, BOARD_VOLCANO);
            setBoardWall(12, 5, BOARD_VOLCANO);
            setBoardWall(12, 6, BOARD_VOLCANO);
            setBoardWall(12, 7, BOARD_VOLCANO);
            setBoardWall(13, 7, BOARD_VOLCANO);

            setBoardWall(14, 7, BOARD_VOLCANO);
            setBoardWall(14, 8, BOARD_VOLCANO);
            setBoardWall(14, 9, BOARD_VOLCANO);

            setBoardWall(7, 10, BOARD_VOLCANO);
            setBoardWall(8, 10, BOARD_VOLCANO);
            setBoardWall(9, 10, BOARD_VOLCANO);
            setBoardWall(11, 10, BOARD_VOLCANO);
            setBoardWall(12, 10, BOARD_VOLCANO);
            setBoardWall(13, 10, BOARD_VOLCANO);
            setBoardWall(14, 10, BOARD_VOLCANO);

            setBoardWall(7, 5, BOARD_VOLCANO);
            setBoardWall(7, 6, BOARD_VOLCANO);
            setBoardWall(7, 7, BOARD_VOLCANO);
            setBoardWall(7, 8, BOARD_VOLCANO);
            setBoardWall(7, 9, BOARD_VOLCANO);

            setBoardNothing(9, 9);
            setBoardNothing(9, 8);
            setBoardNothing(9, 7);
            setBoardNothing(9, 6);
            setBoardNothing(9, 5);

            setBoardNothing(12, 9);
            setBoardNothing(12, 8);
            setBoardNothing(12, 7);
            setBoardNothing(12, 6);
            setBoardNothing(12, 5);

            setBoardNothing(13, 9);
            setBoardNothing(13, 8);
            setBoardNothing(13, 7);
            setBoardNothing(13, 6);
            setBoardNothing(13, 5);

            setBoardNothing(14, 9);
            setBoardNothing(14, 8);
            setBoardNothing(14, 7);
            setBoardNothing(14, 6);
            setBoardNothing(14, 5);

            setBoardNothing(15, 9);
            setBoardNothing(15, 8);
            setBoardNothing(15, 7);
            setBoardNothing(15, 6);
            setBoardNothing(15, 5);

            setBoardNothing(11, 9);
            setBoardNothing(11, 8);
            setBoardNothing(11, 7);
            setBoardNothing(11, 6);
            setBoardNothing(11, 5);

            setBoardNothing(10, 9);
            setBoardNothing(10, 8);
            setBoardNothing(10, 7);
            setBoardNothing(10, 6);
            setBoardNothing(10, 5);
            setBoardWall(10, 10, BOARD_VOLCANO);
            setBoardWall(10, 4, BOARD_VOLCANO);

            setBoardWall(5, 1, BOARD_VOLCANO);
            setBoardWall(6, 1, BOARD_VOLCANO);
            setBoardWall(7, 1, BOARD_VOLCANO);
            setBoardWall(8, 1, BOARD_VOLCANO);
            setBoardWall(9, 1, BOARD_VOLCANO);
            setBoardWall(10, 1, BOARD_VOLCANO);
            setBoardWall(11, 1, BOARD_VOLCANO);
            setBoardWall(12, 1, BOARD_VOLCANO);
            setBoardWall(13, 1, BOARD_VOLCANO);
            setBoardWall(14, 1, BOARD_VOLCANO);

            setBoardWall(3, 4, BOARD_VOLCANO);
            setBoardWall(3, 5, BOARD_VOLCANO);
            setBoardWall(3, 6, BOARD_VOLCANO);
            setBoardWall(3, 7, BOARD_VOLCANO);
            setBoardWall(3, 8, BOARD_VOLCANO);
            setBoardWall(3, 9, BOARD_VOLCANO);

            setBoardWall(4, 4, BOARD_VOLCANO);
            setBoardWall(4, 9, BOARD_VOLCANO);

            setBoardWall(5, 4, BOARD_VOLCANO);
            setBoardWall(5, 5, BOARD_VOLCANO);
            setBoardWall(5, 6, BOARD_VOLCANO);
            setBoardWall(5, 7, BOARD_VOLCANO);
            setBoardWall(5, 8, BOARD_VOLCANO);
            setBoardWall(5, 9, BOARD_VOLCANO);

            setBoardWall(3, 11, BOARD_VOLCANO);
            setBoardWall(3, 12, BOARD_VOLCANO);
            setBoardWall(3, 13, BOARD_VOLCANO);

            setBoardWall(5, 11, BOARD_VOLCANO);
            setBoardWall(5, 12, BOARD_VOLCANO);
            setBoardWall(6, 12, BOARD_VOLCANO);
            setBoardWall(7, 12, BOARD_VOLCANO);
            setBoardWall(8, 12, BOARD_VOLCANO);
            setBoardWall(9, 12, BOARD_VOLCANO);
            setBoardWall(10, 12, BOARD_VOLCANO);
            setBoardWall(10, 11, BOARD_VOLCANO);
            setBoardWall(5, 13, BOARD_VOLCANO);

            setBoardWall(1, 4, BOARD_VOLCANO);
            setBoardWall(1, 5, BOARD_VOLCANO);
            setBoardWall(1, 6, BOARD_VOLCANO);
            setBoardWall(1, 7, BOARD_VOLCANO);
            setBoardWall(1, 8, BOARD_VOLCANO);
            setBoardWall(1, 9, BOARD_VOLCANO);

            setBoardWall(14, 13, BOARD_VOLCANO);
            setBoardWall(13, 13, BOARD_VOLCANO);
            setBoardWall(12, 13, BOARD_VOLCANO);
            setBoardWall(12, 14, BOARD_VOLCANO);

            setBoardWall(13, 4, BOARD_VOLCANO);
            setBoardWall(14, 4, BOARD_VOLCANO);

            putSnake(1, 1);
            game.snake.length = 6;
            moveSnake(1, 0);
            moveSnake(1, 0);
            moveSnake(1, 0);
            moveSnake(0, 1);
            moveSnake(1, 0);
            break;
        default:
            break;
    }
}
void gameSceneExit()
{
    imagePoolFreeAll();
    animationPoolFreeAll();
}
void gameLoseInit()
{
    game.loseTextImg = imageLoad("./img/losemap.png");

    musicPlay(game.lostMus, 0, 1, __LINE__);

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
    game.transitionTimer = 800;
    game.gotostate = state;
    if (game.gotostate >= GAME_PUZZLE1 && game.gotostate <= GAME_PUZZLE12)
    {
        game.current_level = game.gotostate - GAME_PUZZLE1;
    }
}
void gameHandleGoTo()
{
    if (game.gotostate == game.state)
        return;

    // breaks maps for some reason
    // if (game.transitionTimer > 0)
    // {
    //     game.transitionTimer -= tick_speed;
    //     return;
    // }

    switch(game.state)
    {
        case GAME_MENU:
            if (game.gotostate != GAME_SELECT)
                gameSceneExit();
            break;
        case GAME_SELECT:
            if (game.gotostate != GAME_MENU)
                gameSceneExit();
            break;
        case GAME_CLASSIC:
            if (game.gotostate != GAME_LOST_CLASSIC)
                gameSceneExit();
            break;
        case GAME_PUZZLE1:
        case GAME_PUZZLE2:
        case GAME_PUZZLE3:
        case GAME_PUZZLE4:
        case GAME_PUZZLE5:
        case GAME_PUZZLE6:
        case GAME_PUZZLE7:
        case GAME_PUZZLE8:
        case GAME_PUZZLE9:
        case GAME_PUZZLE10:
        case GAME_PUZZLE11:
        case GAME_PUZZLE12:
            if (game.gotostate != GAME_LOST_PUZZLE && game.gotostate != GAME_WON)
                gameSceneExit();
            break;
        case GAME_WON:
        case GAME_LOST_CLASSIC:
        case GAME_LOST_PUZZLE:
            gameSceneExit();
            break;
        default:
            break;
    }

    switch(game.gotostate)
    {
        case GAME_MENU:
            gameMenuInit();
            break;
        case GAME_SELECT:
            if (game.state != GAME_MENU)
            {
                gameMenuInit();
            }
            gameSelectInit();
            break;
        case GAME_CLASSIC:
            gameClassicInit();
            break;
        case GAME_PUZZLE1:
        case GAME_PUZZLE2:
        case GAME_PUZZLE3:
        case GAME_PUZZLE4:
        case GAME_PUZZLE5:
        case GAME_PUZZLE6:
        case GAME_PUZZLE7:
        case GAME_PUZZLE8:
        case GAME_PUZZLE9:
        case GAME_PUZZLE10:
        case GAME_PUZZLE11:
        case GAME_PUZZLE12:
            gamePuzzleInit();
            break;
        case GAME_LOST_CLASSIC:
        case GAME_LOST_PUZZLE:
            gameLoseInit();
            break;
        case GAME_WON:
            musicPlay(game.winMus, 0, 1, __LINE__);
            break;
        default:
            break;
    }
    game.state = game.gotostate;
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
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadNorthWestAnim, __LINE__);
                }
                else if (game.snake.next_move_direction == EAST)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadNorthEastAnim, __LINE__);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadNorthAnim, __LINE__);
                }
                break;
            case SOUTH:
                if (game.snake.next_move_direction == WEST)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadSouthWestAnim, __LINE__);
                }
                else if (game.snake.next_move_direction == EAST)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadSouthEastAnim, __LINE__);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadSouthAnim, __LINE__);
                }
                break;
            case WEST:
                if (game.snake.next_move_direction == NORTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadWestNorthAnim, __LINE__);
                }
                else if (game.snake.next_move_direction == SOUTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadWestSouthAnim, __LINE__);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadWestAnim, __LINE__);
                }
                break;
            case EAST:
                if (game.snake.next_move_direction == NORTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadEastNorthAnim, __LINE__);
                }
                else if (game.snake.next_move_direction == SOUTH)
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadEastSouthAnim, __LINE__);
                }
                else
                {
                    imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadEastAnim, __LINE__);
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
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 0, 3, __LINE__);
                break;
            case SOUTH:
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 1, __LINE__);
                break;
            case WEST:
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 5, 3, __LINE__);
                break;
            case EAST:
                imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 4, 0, __LINE__);
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
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 1, __LINE__);
                }
                else if(next_move_dir == EAST)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 2, __LINE__);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 0, 1, __LINE__);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 0, 2, __LINE__);
                }
                break;
            case SOUTH:
                if (next_move_dir == WEST)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 1, 2, __LINE__);
                }
                else if(next_move_dir == EAST)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 2, 2, __LINE__);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 2, __LINE__);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 3, __LINE__);
                }
                break;
            case WEST:
                if (next_move_dir == NORTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 2, 2, __LINE__);
                }
                else if(next_move_dir == SOUTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 2, __LINE__);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 3, __LINE__);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 4, 3, __LINE__);
                }
                break;
            case EAST:
                if (next_move_dir == NORTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 1, 2, __LINE__);
                }
                else if(next_move_dir == SOUTH)
                {
                    imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 3, 1, __LINE__);
                }
                else
                {
                    if (i % 2 == 0)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 5, 0, __LINE__);
                    if (i % 2 == 1)
                        imageDraw(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, 6, 0, __LINE__);
                }
                break;
            default:
                break;
            }
        }
    }
}
void updateClassicFood()
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
    if (game.state == GAME_CLASSIC)
    {
        for (i = 0; i < game.foodcount; ++i)
        {
            if (game.foods[i].eaten)
                continue;

            imageDraw(game.grassBoardImg, game.foods[i].x, game.foods[i].y, 3, 2, __LINE__);
        }
        return;
    }
    for (i = 0; i < game.foodcount; ++i)
    {
        if (game.foods[i].eaten)
            continue;

        if (game.current_level > 3 && game.current_level < 8)
        {
            imageDraw(game.sandBoardImg, game.foods[i].x, game.foods[i].y, 2, 1, __LINE__);
        } else if (game.current_level > 7)
        {
            imageDraw(game.volcanoBoardImg, game.foods[i].x, game.foods[i].y, 0, 2, __LINE__);
        } else
        {
            imageDraw(game.grassBoardImg, game.foods[i].x, game.foods[i].y, 3, 2, __LINE__);
        }
    }
}
void drawBoard()
{
    int x, y;

    if (game.state == GAME_CLASSIC)
    {
        for (y = game.board.top; y < game.board.bottom; ++y)
        {
            for (x = game.board.left; x < game.board.right; ++x)
            {
                if (game.board.wall[x][y] == -1)
                {
                    continue;
                }
                imageDraw(game.grassBoardImg, x, y, game.board.texture[x][y][0], game.board.texture[x][y][1], __LINE__);
            }
        }
        return;
    }

    switch(game.current_level)
    {
    case 4:
    case 5:
    case 6:
    case 7:
        // remember 0 is top and 16 or something is bottom
        for (y = game.board.top; y < game.board.bottom; ++y)
        {
            for (x = game.board.left; x < game.board.right; ++x)
            {
                if (game.board.wall[x][y] == -1)
                {
                    continue;
                }
                imageDraw(game.sandBoardImg, x, y, game.board.texture[x][y][0], game.board.texture[x][y][1], __LINE__);
            }
        }
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        for (y = game.board.top; y < game.board.bottom; ++y)
        {
            for (x = game.board.left; x < game.board.right; ++x)
            {
                if (game.board.wall[x][y] == -1)
                {
                    continue;
                }
                if (game.board.texture[x][y][0] == 0 && game.board.texture[x][y][1] == 0)
                {
                    imageDrawAnimated(game.volcanoBoardImg, x, y, game.volcanoWallAnim, __LINE__);
                }
                else if (game.board.texture[x][y][0] == 0 && game.board.texture[x][y][1] == 1)
                {
                    imageDrawAnimated(game.volcanoBoardImg, x, y, game.volcanoWall2Anim, __LINE__);
                }
                else
                {
                    imageDraw(game.volcanoBoardImg, x, y, game.board.texture[x][y][0], game.board.texture[x][y][1], __LINE__);
                }
            }
        }
        break;
    default:
        for (y = game.board.top; y < game.board.bottom; ++y)
        {
            for (x = game.board.left; x < game.board.right; ++x)
            {
                if (game.board.wall[x][y] == -1)
                {
                    continue;
                }
                imageDraw(game.grassBoardImg, x, y, game.board.texture[x][y][0], game.board.texture[x][y][1], __LINE__);
            }
        }
        break;
    }
}

void classicRules()
{
    int i, x, y;

    for (y = game.board.top; y < game.board.bottom; ++y)
    {
        for (x = game.board.left; x < game.board.right; ++x)
        {
            if (game.board.wall[x][y] == 0)
                continue;

            if (game.snake.parts[0].x == x && game.snake.parts[0].y == y)
            {
                gameGoTo(GAME_LOST_CLASSIC);
            }
        }
    }

    for (i = 1; i < game.snake.length; ++i)
    {
        if (game.snake.parts[0].x == game.snake.parts[i].x &&
            game.snake.parts[0].y == game.snake.parts[i].y)
        {
            gameGoTo(GAME_LOST_CLASSIC);
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

            Mix_PlayChannel(-1, game.eatSnd, 0);
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
void puzzleRules()
{
    int i, x, y;

    int won = 1;
    for (i = 0; i < game.foodcount; ++i)
    {
        if (!game.foods[i].eaten)
        {
            won = 0;
        }
    }
    if (won)
    {
        gameGoTo(GAME_WON);
    }

    for (y = game.board.top; y < game.board.bottom; ++y)
    {
        for (x = game.board.left; x < game.board.right; ++x)
        {
            if (game.board.wall[x][y] == 0)
                continue;

            if (game.snake.parts[0].x == x && game.snake.parts[0].y == y)
            {
                gameGoTo(GAME_LOST_PUZZLE);
            }
        }
    }

    for (i = 1; i < game.snake.length; ++i)
    {
        if (game.snake.parts[0].x == game.snake.parts[i].x &&
            game.snake.parts[0].y == game.snake.parts[i].y)
        {
            gameGoTo(GAME_LOST_PUZZLE);
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

            Mix_PlayChannel(-1, game.eatSnd, 0);
        }
    }
}
void menuRules()
{
    settleCursor(&game.menuCursor);

    if (game.menuCursor.x > 15)
        game.menuCursor.x = 15;
    if (game.menuCursor.x < 0)
        game.menuCursor.x = 0;
    if (game.menuCursor.y > 15)
        game.menuCursor.y = 15;
    if (game.menuCursor.y < 0)
        game.menuCursor.y = 0;

    if (game.menuCursor.x > 4 && game.menuCursor.x < 11 && game.menuCursor.y >= 9 && game.menuCursor.y <= 10)
    {
        alertCursor(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_SELECT);
    }
    if (game.menuCursor.x > 4 && game.menuCursor.x < 12 && game.menuCursor.y >= 12 && game.menuCursor.y <= 13)
    {
        alertCursor(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_CLASSIC);
    }
    if (game.menuCursor.x > 11 && game.menuCursor.y > 14)
    {
        alertCursor(&game.menuCursor);
        if (game.menuCursor.activated)
        {
            disableCursor(&game.menuCursor);
            game.running = false;
        }
    }
}
void selectRules()
{
    settleCursor(&game.menuCursor);

    if (game.menuCursor.x > 15)
        game.menuCursor.x = 15;
    if (game.menuCursor.x < 0)
        game.menuCursor.x = 0;
    if (game.menuCursor.y > 15)
        game.menuCursor.y = 15;
    if (game.menuCursor.y < 0)
        game.menuCursor.y = 0;

    // grass
    if (game.menuCursor.x > 1 && game.menuCursor.x < 4 && game.menuCursor.y > 5 && game.menuCursor.y < 8)
    {
        if (game.unlocked_levelcount > 0)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE1);
        }
    }
    if (game.menuCursor.x > 4 && game.menuCursor.x < 7 && game.menuCursor.y > 5 && game.menuCursor.y < 8)
    {
        if (game.unlocked_levelcount > 1)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE2);
        }
    }
    if (game.menuCursor.x > 8 && game.menuCursor.x < 11 && game.menuCursor.y > 5 && game.menuCursor.y < 8)
    {
        if (game.unlocked_levelcount > 2)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE3);
        }
    }
    if (game.menuCursor.x > 11 && game.menuCursor.x < 14 && game.menuCursor.y > 5 && game.menuCursor.y < 8)
    {
        if (game.unlocked_levelcount > 3)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE4);
        }
    }

    // sandbox
    if (game.menuCursor.x > 1 && game.menuCursor.x < 4 && game.menuCursor.y > 8 && game.menuCursor.y < 11)
    {
        if (game.unlocked_levelcount > 4)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE5);
        }
    }
    if (game.menuCursor.x > 4 && game.menuCursor.x < 7 && game.menuCursor.y > 8 && game.menuCursor.y < 11)
    {
        if (game.unlocked_levelcount > 5)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE6);
        }
    }
    if (game.menuCursor.x > 8 && game.menuCursor.x < 11 && game.menuCursor.y > 8 && game.menuCursor.y < 11)
    {
        if (game.unlocked_levelcount > 6)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE7);
        }
    }
    if (game.menuCursor.x > 11 && game.menuCursor.x < 14 && game.menuCursor.y > 8 && game.menuCursor.y < 11)
    {
        if (game.unlocked_levelcount > 7)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE8);
        }
    }

    // volcano
    if (game.menuCursor.x > 1 && game.menuCursor.x < 4 && game.menuCursor.y > 11 && game.menuCursor.y < 14)
    {
        if (game.unlocked_levelcount > 8)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE9);
        }
    }
    if (game.menuCursor.x > 4 && game.menuCursor.x < 7 && game.menuCursor.y > 11 && game.menuCursor.y < 14)
    {
        if (game.unlocked_levelcount > 9)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE10);
        }
    }
    if (game.menuCursor.x > 8 && game.menuCursor.x < 11 && game.menuCursor.y > 11 && game.menuCursor.y < 14)
    {
        if (game.unlocked_levelcount > 10)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE11);
        }
    }
    if (game.menuCursor.x > 11 && game.menuCursor.x < 14 && game.menuCursor.y > 11 && game.menuCursor.y < 14)
    {
        if (game.unlocked_levelcount > 11)
        {
            alertCursor(&game.menuCursor);
            if (game.menuCursor.activated)
                gameGoTo(GAME_PUZZLE12);
        }
    }
}
void drawMenuCursor()
{
    if (game.menuCursor.alert)
    {
        if (game.menuCursor.activated)
            imageDraw(game.menuImg, game.menuCursor.x, game.menuCursor.y, 17, 3, __LINE__);
        if (!game.menuCursor.activated)
            imageDraw(game.menuImg, game.menuCursor.x, game.menuCursor.y, 16, 3, __LINE__);
    }
    else if (game.menuCursor.disabled)
    {
        imageDrawAnimated(game.menuImg, game.menuCursor.x, game.menuCursor.y, game.menuCursorWaitAnim, __LINE__);
    }
    else
        imageDrawAnimated(game.menuImg, game.menuCursor.x, game.menuCursor.y, game.menuCursorAnim, __LINE__);
}
void drawPuzzleObjective(char *textbuf)
{
    switch(game.current_level)
    {
        case 0:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 2);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 1:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 4);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 2:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 6);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 3:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 9);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 4:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 2);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 5:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 6);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 6:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 7);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 7:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 4, 1);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 8:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 2);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 9:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 2, 9);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 10:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 3, 9);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 11:
            sprintf(textbuf, "%s %i/%02i", "FOOD:", game.snake.length - 6, 9);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        default:
            break;
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
                // keystates[event->key.keysym.scancode] = KEY_RELEASED;
                keystates[event->key.keysym.scancode] = KEY_JUSTRELEASED;
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
void handleLostClassicInput()
{
    if (getAnyKey() && !getKey(SDL_SCANCODE_ESCAPE) && game.snake.parts[game.snake.length - 1].destroyed)
    {
        gameGoTo(GAME_CLASSIC);
    }
}
void handleLostPuzzleInput()
{
    if (getAnyKey() && !getKey(SDL_SCANCODE_ESCAPE) && game.snake.parts[game.snake.length - 1].destroyed)
    {
        gameGoTo(GAME_PUZZLE1 + game.current_level);
    }
}
void handleWonInput()
{
    if (getAnyKey() && !getKey(SDL_SCANCODE_ESCAPE) && game.winAnim->frame == game.winAnim->framecount - 1)
    {
        if (game.current_level == 11)
        {
            gameGoTo(GAME_SELECT);
        }
        else
        {
            gameGoTo(GAME_PUZZLE1 + game.current_level + 1);
        }
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
void handlePuzzleInput()
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

    if (getKey(SDL_SCANCODE_W) == KEY_JUSTRELEASED || getKey(SDL_SCANCODE_UP) == KEY_JUSTRELEASED)
    {
        moveSnake(0, -1);
    }
    if (getKey(SDL_SCANCODE_S) == KEY_JUSTRELEASED || getKey(SDL_SCANCODE_DOWN) == KEY_JUSTRELEASED)
    {
        moveSnake(0, 1);
    }
    if (getKey(SDL_SCANCODE_A) == KEY_JUSTRELEASED || getKey(SDL_SCANCODE_LEFT) == KEY_JUSTRELEASED)
    {
        moveSnake(-1, 0);
    }
    if (getKey(SDL_SCANCODE_D) == KEY_JUSTRELEASED || getKey(SDL_SCANCODE_RIGHT) == KEY_JUSTRELEASED)
    {
        moveSnake(1, 0);
    }
}
void handleMenuInput()
{
    if (getKey(SDL_SCANCODE_W) == KEY_TAPPED || getKey(SDL_SCANCODE_UP) == KEY_TAPPED)
    {
        moveCursor(&game.menuCursor, 0, -1);
    }
    if (getKey(SDL_SCANCODE_S) == KEY_TAPPED || getKey(SDL_SCANCODE_DOWN) == KEY_TAPPED)
    {
        moveCursor(&game.menuCursor, 0, 1);
    }
    if (getKey(SDL_SCANCODE_A) == KEY_TAPPED || getKey(SDL_SCANCODE_LEFT) == KEY_TAPPED)
    {
        moveCursor(&game.menuCursor, -1, 0);
    }
    if (getKey(SDL_SCANCODE_D) == KEY_TAPPED || getKey(SDL_SCANCODE_RIGHT) == KEY_TAPPED)
    {
        moveCursor(&game.menuCursor, 1, 0);
    }
    if (getKey(SDL_SCANCODE_SPACE) == KEY_TAPPED || getKey(SDL_SCANCODE_RETURN) == KEY_TAPPED || getKey(SDL_SCANCODE_Z) == KEY_TAPPED)
    {
        activateCursor(&game.menuCursor);
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
            imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakePartExplodeAnim, __LINE__);
            Mix_PlayChannel(-1, game.explodeSnd, 0);
            break;
        }
    }
}

void drawLevelIcons()
{
    // grass
    if (game.unlocked_levelcount < 1)
    {
        imageDrawLarge(game.selectImg, 2, 6, 16, 0, 2, 2, __LINE__);
    }
    else if (game.unlocked_levelcount == 1)
    {
        imageDrawLarge(game.selectImg, 2, 6, 20, 4, 2, 2, __LINE__);
    }
    else
    {
        imageDrawLarge(game.selectImg, 2, 6, 22, 4, 2, 2, __LINE__);
    }
    if (game.unlocked_levelcount < 2)
    {
        imageDrawLarge(game.selectImg, 5, 6, 16, 0, 2, 2, __LINE__);
    }
    else if (game.unlocked_levelcount == 2)
    {
        imageDrawLarge(game.selectImg, 5, 6, 28, 4, 2, 2, __LINE__);
    }
    else
    {
        imageDrawLarge(game.selectImg, 5, 6, 30, 4, 2, 2, __LINE__);
    }
    if (game.unlocked_levelcount < 3)
    {
        imageDrawLarge(game.selectImg, 9, 6, 16, 0, 2, 2, __LINE__);
    }
    else if (game.unlocked_levelcount == 3)
    {
        imageDrawLarge(game.selectImg, 9, 6, 20, 6, 2, 2, __LINE__);
    }
    else
    {
        imageDrawLarge(game.selectImg, 9, 6, 22, 6, 2, 2, __LINE__);
    }
    if (game.unlocked_levelcount < 4)
    {
        imageDrawLarge(game.selectImg, 12, 6, 16, 0, 2, 2, __LINE__);
    }
    else if (game.unlocked_levelcount == 4)
    {
        imageDrawLarge(game.selectImg, 12, 6, 28, 6, 2, 2, __LINE__);
    }
    else
    {
        imageDrawLarge(game.selectImg, 12, 6, 30, 6, 2, 2, __LINE__);
    }

    // sandbox
    if (game.unlocked_levelcount > 4)
    {
        if (game.unlocked_levelcount < 5)
        {
            imageDrawLarge(game.selectImg, 2, 9, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 5)
        {
            imageDrawLarge(game.selectImg, 2, 9, 20, 8, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 2, 9, 22, 8, 2, 2, __LINE__);
        }
        if (game.unlocked_levelcount < 6)
        {
            imageDrawLarge(game.selectImg, 5, 9, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 6)
        {
            imageDrawLarge(game.selectImg, 5, 9, 28, 8, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 5, 9, 30, 8, 2, 2, __LINE__);
        }
        if (game.unlocked_levelcount < 7)
        {
            imageDrawLarge(game.selectImg, 9, 9, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 7)
        {
            imageDrawLarge(game.selectImg, 9, 9, 20, 10, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 9, 9, 22, 10, 2, 2, __LINE__);
        }
        if (game.unlocked_levelcount < 8)
        {
            imageDrawLarge(game.selectImg, 12, 9, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 8)
        {
            imageDrawLarge(game.selectImg, 12, 9, 28, 10, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 12, 9, 30, 10, 2, 2, __LINE__);
        }
    }
    // volcano
    if (game.unlocked_levelcount > 8)
    {
        if (game.unlocked_levelcount < 9)
        {
            imageDrawLarge(game.selectImg, 2, 12, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 9)
        {
            imageDrawLarge(game.selectImg, 2, 12, 20, 12, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 2, 12, 22, 12, 2, 2, __LINE__);
        }
        if (game.unlocked_levelcount < 10)
        {
            imageDrawLarge(game.selectImg, 5, 12, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 10)
        {
            imageDrawLarge(game.selectImg, 5, 12, 28, 12, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 5, 12, 30, 12, 2, 2, __LINE__);
        }
        if (game.unlocked_levelcount < 11)
        {
            imageDrawLarge(game.selectImg, 9, 12, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 11)
        {
            imageDrawLarge(game.selectImg, 9, 12, 20, 14, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 9, 12, 22, 14, 2, 2, __LINE__);
        }
        if (game.unlocked_levelcount < 12)
        {
            imageDrawLarge(game.selectImg, 12, 12, 16, 0, 2, 2, __LINE__);
        }
        else if (game.unlocked_levelcount == 12)
        {
            imageDrawLarge(game.selectImg, 12, 12, 28, 14, 2, 2, __LINE__);
        }
        else
        {
            imageDrawLarge(game.selectImg, 12, 12, 30, 14, 2, 2, __LINE__);
        }
    }
    imageDrawLarge(game.selectImg, 1, 2, 16, 2, 14, 2, __LINE__);
}

void gameLoop()
{
    SDL_RenderClear(app.renderer);

    updateKeys();
    sdlEventLoop(&event);
    handleGeneralInput();

    int game_caught_up = 0;
    gameUpdateTime();

    musicHandler();
    gameHandleGoTo();

    char textbuf[128];

    switch(game.state)
    {
    case GAME_MENU:
        deactivateCursor(&game.menuCursor);
        handleMenuInput();
        
        while (!game_caught_up)
        {
            menuRules();
            animationPoolUpdateAll();

            game_caught_up = gameCaughtUp();
        }

        imageDrawLarge(game.menuImg, 0, 0, 0, 0, 16, 16, __LINE__);
        imageDrawLargeAnimated(game.menuImg, 5, 6, game.menuSnakeBlinkAnim, 2, 2, __LINE__);
        imageDrawLargeAnimated(game.menuImg, 2, 1, game.menuTitleShineAnim, 11, 4, __LINE__);

        imageDrawLarge(game.menuImg, 12, 14, 20, 0, 4, 2, __LINE__);

        imageDrawLarge(game.menuImg, 5, 9, 24, 0, 6, 2, __LINE__);
        imageDrawLarge(game.menuImg, 5, 12, 24, 2, 7, 2, __LINE__);
        drawMenuCursor();
        break;
    case GAME_CLASSIC:
        handleClassicInput();
        while (!game_caught_up)
        {
            classicRules();
            updateClassicFood();
            animationPoolUpdateAll();

            game_caught_up = gameCaughtUp();
        }

        drawBoard();
        drawFood();
        drawSnake();
        sprintf(textbuf, "%s%d", "SCORE: ", game.snake.length - 2);
        imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
        break;
    case GAME_SELECT:
        deactivateCursor(&game.menuCursor);
        handleMenuInput();
        while (!game_caught_up)
        {
            selectRules();
            animationPoolUpdateAll();

            game_caught_up = gameCaughtUp();
        }

        imageDrawLargeAnimated(game.selectImg, 0, 0, game.selectBGAnim, 8, 8, __LINE__);
        imageDrawLargeAnimated(game.selectImg, 8, 0, game.selectBGAnim, 8, 8, __LINE__);
        imageDrawLargeAnimated(game.selectImg, 0, 8, game.selectBGAnim, 8, 8, __LINE__);
        imageDrawLargeAnimated(game.selectImg, 8, 8, game.selectBGAnim, 8, 8, __LINE__);

        drawLevelIcons();
        drawMenuCursor();
        break;
    case GAME_PUZZLE1:
    case GAME_PUZZLE2:
    case GAME_PUZZLE3:
    case GAME_PUZZLE4:
    case GAME_PUZZLE5:
    case GAME_PUZZLE6:
    case GAME_PUZZLE7:
    case GAME_PUZZLE8:
    case GAME_PUZZLE9:
    case GAME_PUZZLE10:
    case GAME_PUZZLE11:
    case GAME_PUZZLE12:
        handlePuzzleInput();
        while (!game_caught_up)
        {
            puzzleRules();
            animationPoolUpdateAll();

            game_caught_up = gameCaughtUp();
        }

        drawBoard();
        drawFood();
        drawSnake();
        drawSnake();
        drawPuzzleObjective(textbuf);
        break;
    case GAME_LOST_CLASSIC:
        handleLostClassicInput();
        while (!game_caught_up)
        {
            animationPoolUpdateAll();
            game_caught_up = gameCaughtUp();
        }
        snakeExplode();
        drawSnake();
        if (game.snake.parts[game.snake.length - 1].destroyed)
        {
            animationUnpause(game.loseAnim);
            imageDrawLargeAnimated(game.loseTextImg, 0, 4, game.loseAnim, 16, 2, __LINE__);
            if (game.loseAnim->frame == game.loseAnim->framecount - 1)
            {
                imageDrawLarge(game.loseTextImg, 0, 6, 0, 30, 16, 2, __LINE__);
            }
        }
        imageDrawText(textbuf, &game.font, game.loseAnim->frame / 3, (float)game.loseAnim->frame / 1.5f, __LINE__);
        imageDrawText("ESC TO RETURN", &game.font, 2, 14, __LINE__);
        break;
    case GAME_LOST_PUZZLE:
        handleLostPuzzleInput();
        while (!game_caught_up)
        {
            animationPoolUpdateAll();
            game_caught_up = gameCaughtUp();
        }
        snakeExplode();
        drawSnake();
        if (game.snake.parts[game.snake.length - 1].destroyed)
        {
            animationUnpause(game.loseAnim);
            imageDrawLargeAnimated(game.loseTextImg, 0, 4, game.loseAnim, 16, 2, __LINE__);
            if (game.loseAnim->frame == game.loseAnim->framecount - 1)
            {
                imageDrawLarge(game.loseTextImg, 0, 6, 0, 30, 16, 2, __LINE__);
            }
        }
        imageDrawText(textbuf, &game.font, game.loseAnim->frame / 3, (float)game.loseAnim->frame / 1.5f, __LINE__);
        imageDrawText("ESC TO RETURN", &game.font, 2, 14, __LINE__);
        break;
    case GAME_WON:
        handleWonInput();
        while (!game_caught_up)
        {
            animationPoolUpdateAll();
            game_caught_up = gameCaughtUp();
        }

        if (game.current_level == game.unlocked_levelcount - 1)
        {
            game.unlocked_levelcount++;
        }
        animationUnpause(game.winAnim);
        drawBoard();
        drawFood();
        drawSnake();
        // imageDrawLargeAnimated(game.winTextImg, 0, 4, game.winAnim, 16, 2, __LINE__);
        imageDrawText("YOU WIN", &game.font, 5, 4, __LINE__);
        imageDrawText("PRESS ANY KEY", &game.font, 2, 7, __LINE__);
        if (game.winAnim->frame == game.winAnim->framecount - 1)
        {
            imageDrawLarge(game.winTextImg, 0, 6, 0, 30, 16, 2, __LINE__);
        }
        imageDrawText(textbuf, &game.font, game.winAnim->frame / 2, (float)game.winAnim->frame * 1.5, __LINE__);
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
