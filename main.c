#include "definitions.h"
#include "graphics.h"
#include "audio.h"
#include "application.h"

#define window_width 1280
#define window_height 720
#define screen_width 128
#define screen_height 128

// game plan

// apply non-animated snake graphics in the code
// make animation system be apply-and-forget, by making an imageAnimate function where you specify the animation components (cont.)
//      and the imageDraw function handles the animating sprite by checking if there's an animation in play and picking the appropriate (cont.)
//      sprite to draw.

/*

    // behind the scenes: hold a specific animation pointer that is the "current edited animation"
    // (note: the frameaction and framesound functions are theoretical, don't implement them today)
    beginAnimation(&snakeAnim);
    addFrame(0, 0, 100); // hold sprite texture position 0, 0 for 100 ticks
    // addFrameAction((void *)funcFoo()); // hmm
    addFrame(1, 0, 100);
    // addFrameSound(&snakeHissSnd); // could also be done with the frameAction but I think having a dedicated function for this is worth (You'll probably be adding a bunch of sounds to animations)
    // examples of animations that could have sounds play on a frame:
    //      snake hisses on the frame he sticks his tongue out
    //      player step sound
    //      title card shine sound when the shine frames play
    //      there's an endless selection, endless - brian regan
    endAnimation(LOOPING); // apply final animation properties with a bitflag (like whether it loops) (in fact just make this the loopcheck rn)
    // other animation flags in the future could be REVERSE (MIRRORED), PING-PONG, more if you think of them

    imageDrawAnimated(&snakeImg, &snakeAnim);
*/

#define MAX_ANIMATION_FRAMES 16
typedef struct _Animation
{
    int xframes[MAX_ANIMATION_FRAMES];
    int yframes[MAX_ANIMATION_FRAMES];
    int timeframes[MAX_ANIMATION_FRAMES];
    unsigned int framecount;
    
    int time;
    unsigned int frame;

    uint8 args;

    struct _Animation *next;
} Animation;

#define MAX_ANIMATIONS 128
typedef struct
{
    Animation animations[MAX_ANIMATIONS];
    unsigned int head;
    Animation *free_animations;

    Animation *editing_animation;
} AnimationPool;

AnimationPool anim_pool;

Animation *AnimationAlloc()
{
    if (anim_pool.head != MAX_ANIMATIONS)
    {
        return &anim_pool.animations[anim_pool.head++];
    }
    if (anim_pool.free_animations)
    {
        Animation *out = anim_pool.free_animations;
        anim_pool.free_animations = anim_pool.free_animations->next;
        return out;
    }

    verify(false, "Ran out of animations, please tell chiefkat to increase the limit or implement a better animation system", __LINE__);
    return NULL;
}
void beginAnimation(Animation *anim)
{
    anim_pool.editing_animation = anim;
    anim_pool.editing_animation->framecount = 0;
}
void addFrame(int x, int y, int time)
{
    if (!anim_pool.editing_animation)
    {
        verify(false, "there is no animation to add frames to. Please specify the editing animation with beginAnimation()", __LINE__);
    }
    
    unsigned int frame = anim_pool.editing_animation->framecount;
    anim_pool.editing_animation->xframes[frame] = x;
    anim_pool.editing_animation->yframes[frame] = y;
    anim_pool.editing_animation->timeframes[frame] = time;
    ++anim_pool.editing_animation->framecount;
}
void endAnimation(uint8 args)
{
    if (!anim_pool.editing_animation)
    {
        verify(false, "there is no animation to end. Please specify the editing animation with beginAnimation()", __LINE__);
    }

    anim_pool.editing_animation->args = args;

    // get the animation ready to start play
    if (anim_pool.editing_animation->framecount > 0)
    {
        anim_pool.editing_animation->time = anim_pool.editing_animation->timeframes[0];
    }


    anim_pool.editing_animation = NULL;
}
void updateAnimation(Animation *anim)
{
    if (anim->framecount <= 0)
        return;

    anim->time -= tick_speed;
    if (anim->time <= 0)
    {
        printf("%d %d\n", anim->time, anim->timeframes[0]);
        ++anim->frame;
        if (anim->frame >= anim->framecount && anim->args == 0)
        {
            anim->time = 0;
            anim->frame = anim->framecount - 1;
            return;
        }
        // loops (make this less hardcoded pls, like a switch statement for args or turn it into a bitflag)
        if (anim->frame >= anim->framecount && anim->args != 0)
        {
            anim->time = anim->timeframes[0];
            anim->frame = 0;
            return;
        }
        anim->time = anim->timeframes[anim->frame];
    }
}

void imageDrawAnimated(Image *img, int x, int y, Animation *anim)
{

    SDL_Rect scale_rect = {x * IMG_PIXEL_SIZE, y * IMG_PIXEL_SIZE, IMG_PIXEL_SIZE, IMG_PIXEL_SIZE};
    img->clip_rect = (SDL_Rect){.x = anim->xframes[anim->frame] * IMG_PIXEL_SIZE,
                                .y = anim->yframes[anim->frame] * IMG_PIXEL_SIZE,
                                .w = IMG_PIXEL_SIZE, .h = IMG_PIXEL_SIZE};
    SDL_RenderCopy(app.renderer, img->img, &img->clip_rect, &scale_rect);
}

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
    Animation *snakeHeadNorthAnim;
    Animation *snakeHeadSouthAnim;
    Animation *snakeHeadWestAnim;
    Animation *snakeHeadEastAnim;
    // Animation *snakeBodyAnim;
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

    game.snakeHeadNorthAnim = AnimationAlloc();
    game.snakeHeadSouthAnim = AnimationAlloc();
    game.snakeHeadWestAnim = AnimationAlloc();
    game.snakeHeadEastAnim = AnimationAlloc();
    // game.snakeBodyAnim = AnimationAlloc();

    beginAnimation(game.snakeHeadNorthAnim);
    addFrame(0, 0, 8000);
    addFrame(1, 0, 8000);
    endAnimation(1);
    beginAnimation(game.snakeHeadSouthAnim);
    addFrame(7, 1, 8000);
    addFrame(8, 1, 8000);
    endAnimation(1);
    beginAnimation(game.snakeHeadWestAnim);
    addFrame(1, 3, 8000);
    addFrame(2, 3, 8000);
    endAnimation(1);
    beginAnimation(game.snakeHeadEastAnim);
    addFrame(7, 0, 8000);
    addFrame(8, 0, 8000);
    endAnimation(1);

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
        {
            switch(game.snake.next_move_direction)
            {
            case NORTH:
                imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadNorthAnim);
                break;
            case SOUTH:
                imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadSouthAnim);
                break;
            case WEST:
                imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadWestAnim);
                break;
            case EAST:
                imageDrawAnimated(game.snakeImg, game.snake.parts[i].x, game.snake.parts[i].y, game.snakeHeadEastAnim);
                break;
            default:
                break;
            }
        }
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
            updateAnimation(game.snakeHeadNorthAnim);
            updateAnimation(game.snakeHeadSouthAnim);
            updateAnimation(game.snakeHeadWestAnim);
            updateAnimation(game.snakeHeadEastAnim);

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