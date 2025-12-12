#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "definitions.h"
#include "application.h"

#define IMG_PIXEL_SIZE 8

typedef struct _Image
{
    SDL_Texture *img;
    int width, height;
    int xcount, ycount;
    SDL_Rect clip_rect;
    struct _Image *next;
} Image;
#define MAX_IMAGES 12
typedef struct
{
    Image images[MAX_IMAGES];
    unsigned int head;
    Image *free_images;
} ImagePool;
extern ImagePool img_pool;

SDL_Texture *loadSurfaceImage(const char *path, int *tile_count_x, int *tile_count_y, int *img_width, int *img_height);

Image *imageAlloc();
void imageLoadSurfaceImage(Image *img, const char *path);
Image *imageLoad(const char *path);
void imageDraw(Image *img, int x, int y, int tx, int ty, int line);
void imageDrawLarge(Image *img, int x, int y, int tx, int ty, int tw, int th, int line);
void imageFree(Image *img);

void imagePoolFreeAll();

#endif