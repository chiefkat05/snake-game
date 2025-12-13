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
void putFood(int x, int y)
{
    x += game.board.left;
    y += game.board.top;
    game.foods[game.foodcount].x = x;
    game.foods[game.foodcount].y = y;
    ++game.foodcount;
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
void gameSelectInit()
{
    game.selectImg = imageLoad("./img/selectmap.png");

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
    game.boardImg = imageLoad("./img/grassmap.png");

    game.lowMus = musicLoad("./snd/testsync.ogg");
    game.highMus = musicLoad("./snd/testsync2.ogg");

    musicPlay(game.lowMus, __LINE__);
    musicPlay(game.highMus, __LINE__);

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

    fontLoad(&game.font, "./img/textmap.png", "0123456789:?!.,-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/");
}
void gameClassicInit()
{
    baseGameInit();

    makeBoard(16, 16);
    setBoardGrass();
    spawnFood(16);
}
void gamePuzzleInit()
{
    baseGameInit();

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

    switch(game.current_level)
    {
        case 0:
            makeBoard(5, 5);
            putFood(1, 2);
            putFood(2, 3);
            break;
        case 1:
            makeBoard(6, 6);
            putFood(2, 2);
            putFood(3, 2);
            putFood(2, 4);
            putFood(4, 3);
            break;
        case 2:
            makeBoard(6, 8);
            putFood(2, 2);
            putFood(2, 2);
            putFood(2, 2);
            putFood(2, 2);
            putFood(2, 2);
            putFood(2, 2);
            putFood(2, 2);
            putFood(2, 2);
            break;
        default:
            break;
    }
    setBoardGrass();
}
void gameSceneExit()
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
    if (game.gotostate >= GAME_PUZZLE1 && game.gotostate <= GAME_PUZZLE3)
    {
        game.current_level = game.gotostate - GAME_PUZZLE1;
    }
}
void gameHandleGoTo()
{
    if (game.gotostate == game.state)
        return;

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
            gamePuzzleInit();
            break;
        case GAME_LOST_CLASSIC:
        case GAME_LOST_PUZZLE:
            gameLoseInit();
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
    for (i = 0; i < game.foodcount; ++i)
    {
        if (game.foods[i].eaten)
            continue;

        imageDraw(game.boardImg, game.foods[i].x, game.foods[i].y, 3, 2, __LINE__);
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
            imageDraw(game.boardImg, x, y, game.board.texture[x][y][0], game.board.texture[x][y][1], __LINE__);
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
        gameGoTo(GAME_LOST_CLASSIC);
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
    int i;
    switch(game.current_level)
    {
    case 0:
        if (game.snake.length >= 4)
        {
            gameGoTo(GAME_WON);
        }
        break;
    case 1:
        if (game.snake.length >= 6)
        {
            gameGoTo(GAME_WON);
        }
        break;
    case 2:
        if (game.snake.length >= 10)
        {
            gameGoTo(GAME_WON);
        }
        break;
    default:
        break;
    }

    if (game.snake.parts[0].x == game.board.left ||
        game.snake.parts[0].x == game.board.right - 1 ||
        game.snake.parts[0].y == game.board.top ||
        game.snake.parts[0].y == game.board.bottom - 1)
    {
        gameGoTo(GAME_LOST_PUZZLE);
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

    if (game.menuCursor.x > 1 && game.menuCursor.x < 8 && game.menuCursor.y >= 8 && game.menuCursor.y <= 10)
    {
        alertCursor(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_SELECT);
    }
    if (game.menuCursor.x > 9 && game.menuCursor.x < 15 && game.menuCursor.y >= 8 && game.menuCursor.y <= 10)
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

    if (game.menuCursor.x > 1 && game.menuCursor.x < 4 && game.menuCursor.y > 5 && game.menuCursor.y < 8)
    {
        alertCursor(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_PUZZLE1);
    }
    if (game.menuCursor.x > 4 && game.menuCursor.x < 7 && game.menuCursor.y > 5 && game.menuCursor.y < 8)
    {
        alertCursor(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_PUZZLE2);
    }
    if (game.menuCursor.x > 8 && game.menuCursor.x < 11 && game.menuCursor.y > 5 && game.menuCursor.y < 8)
    {
        alertCursor(&game.menuCursor);
        if (game.menuCursor.activated)
            gameGoTo(GAME_PUZZLE3);
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
            sprintf(textbuf, "%s %i/%02i", "Length:", game.snake.length - 2, 2);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 1:
            sprintf(textbuf, "%s %i/%02i", "Length:", game.snake.length - 2, 4);
            imageDrawText(textbuf, &game.font, 0, 0, __LINE__);
            break;
        case 2:
            sprintf(textbuf, "%s %i/%02i", "Length:", game.snake.length - 2, 8);
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
        gameGoTo(GAME_SELECT);
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
            break;
        }
    }
}

void gameLoop()
{
    SDL_RenderClear(app.renderer);

    updateKeys();
    sdlEventLoop(&event);
    handleGeneralInput();

    int game_caught_up = 0;
    gameUpdateTime();

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
        imageDrawLarge(game.menuImg, 12, 14, 20, 0, 4, 2, __LINE__);

        imageDrawLarge(game.menuImg, 2, 8, 24, 0, 5, 2, __LINE__);
        imageDrawLarge(game.menuImg, 10, 8, 24, 2, 5, 2, __LINE__);
        drawMenuCursor();
        break;
    case GAME_CLASSIC:
        handleClassicInput();
        while (!game_caught_up)
        {
            classicRules();
            updateClassicFood();
            musicFadeIn(game.lowMus, __LINE__);
            musicFadeIn(game.highMus, __LINE__);
            animationPoolUpdateAll();

            game_caught_up = gameCaughtUp();
        }

        drawBoard();
        drawFood();
        drawSnake();
        sprintf(textbuf, "%s%d", "Score: ", game.snake.length - 2);
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
        // grass
        imageDrawLarge(game.selectImg, 2, 6, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 5, 6, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 9, 6, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 12, 6, 16, 0, 2, 2, __LINE__);
        // sandbox
        imageDrawLarge(game.selectImg, 2, 9, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 5, 9, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 9, 9, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 12, 9, 16, 0, 2, 2, __LINE__);
        // volcano
        imageDrawLarge(game.selectImg, 2, 12, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 5, 12, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 9, 12, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 12, 12, 16, 0, 2, 2, __LINE__);
        imageDrawLarge(game.selectImg, 1, 2, 16, 2, 14, 2, __LINE__);
        drawMenuCursor();
        break;
    case GAME_PUZZLE1:
    case GAME_PUZZLE2:
    case GAME_PUZZLE3:
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
            musicFadeOut(game.lowMus, __LINE__);
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
            imageDrawLargeAnimated(game.loseTextImg, 0, 4, game.loseAnim, 16, 2, __LINE__);
            if (game.loseAnim->frame == game.loseAnim->framecount - 1)
            {
                imageDrawLarge(game.loseTextImg, 0, 6, 0, 30, 16, 2, __LINE__);
            }
        }
        imageDrawText(textbuf, &game.font, game.loseAnim->frame / 3, (float)game.loseAnim->frame / 1.5f, __LINE__);
        break;
    case GAME_LOST_PUZZLE:
        handleLostPuzzleInput();
        while (!game_caught_up)
        {
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
            imageDrawLargeAnimated(game.loseTextImg, 0, 4, game.loseAnim, 16, 2, __LINE__);
            if (game.loseAnim->frame == game.loseAnim->framecount - 1)
            {
                imageDrawLarge(game.loseTextImg, 0, 6, 0, 30, 16, 2, __LINE__);
            }
        }
        imageDrawText(textbuf, &game.font, game.loseAnim->frame / 3, (float)game.loseAnim->frame / 1.5f, __LINE__);
        break;
    case GAME_WON:
        handleWonInput();
        while (!game_caught_up)
        {
            animationPoolUpdateAll();
            game_caught_up = gameCaughtUp();
        }
        animationUnpause(game.winAnim);
        drawBoard();
        drawFood();
        drawSnake();
        imageDrawLargeAnimated(game.winTextImg, 0, 4, game.winAnim, 16, 2, __LINE__);
        if (game.winAnim->frame == game.winAnim->framecount - 1)
        {
            imageDrawLarge(game.winTextImg, 0, 6, 0, 30, 16, 2, __LINE__);
        }
        imageDrawText(textbuf, &game.font, game.winAnim->frame / 3, (float)game.winAnim->frame * 1.5, __LINE__);
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
