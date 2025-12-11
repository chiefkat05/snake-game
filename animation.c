#include "animation.h"

Animation *animationAlloc()
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
void animationFree(Animation *anim)
{
    anim->next = anim_pool.free_animations;
    anim_pool.free_animations = anim;
}
Animation *animationCreate()
{
    Animation *anim = animationAlloc();

    anim_pool.editing_animation = anim;
    anim_pool.editing_animation->framecount = 0;

    return anim;
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
void animationFinish(uint8 args)
{
    if (!anim_pool.editing_animation)
    {
        verify(false, "there is no animation to end. Please specify the editing animation with beginAnimation()", __LINE__);
    }

    anim_pool.editing_animation->args = args;

    if (anim_pool.editing_animation->framecount > 0)
    {
        anim_pool.editing_animation->time = anim_pool.editing_animation->timeframes[0];
    }

    anim_pool.editing_animation = NULL;
}
void animationUpdate(Animation *anim)
{
    if (anim->framecount <= 0)
        return;

    anim->time -= tick_speed;
    if (anim->time <= 0)
    {
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

void animationPoolUpdateAll()
{
    int i;
    for (i = 0; i < MAX_ANIMATIONS; ++i)
    {
        if (anim_pool.animations[i].framecount <= 0)
            continue;

        animationUpdate(&anim_pool.animations[i]);
    }

    Animation *cursor = anim_pool.free_animations;
    while (cursor)
    {
        animationUpdate(cursor);
        cursor = cursor->next;
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
// also imageDrawLargeAnimated when you make imageDrawLarge