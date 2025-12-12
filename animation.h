#ifndef ANIMATION_H
#define ANIMATION_H

#include "graphics.h"

#define MAX_ANIMATION_FRAMES 16
typedef struct _Animation
{
    int xframes[MAX_ANIMATION_FRAMES];
    int yframes[MAX_ANIMATION_FRAMES];
    int timeframes[MAX_ANIMATION_FRAMES];
    unsigned int framecount;
    
    int time;
    unsigned int frame;
    int paused;

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

extern AnimationPool anim_pool;

Animation *animationAlloc();
void animationFree(Animation *anim);
Animation *animationCreate();
void addFrame(int x, int y, int time);
void animationFinish(uint8 args);
void animationUpdate(Animation *anim);

void animationPoolUpdateAll();
void animationPoolFreeAll();
void animationPause(Animation *anim);
void animationUnpause(Animation *anim);

void imageDrawAnimated(Image *img, int x, int y, Animation *anim);
void imageDrawLargeAnimated(Image *img, int x, int y, Animation *anim, int tw, int th);


#endif