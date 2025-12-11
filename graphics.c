#include "graphics.h"

SDL_Texture *loadSurfaceImage(const char *path, int *tile_count_x, int *tile_count_y, int *img_width, int *img_height)
{
    SDL_Surface *img = IMG_Load(path);
    verify(img, "failed to load image", __LINE__);
    SDL_Texture *sprite = SDL_CreateTextureFromSurface(app.renderer, img);
    verify(sprite, "failed to create texture", __LINE__);

    *img_width = img->w;
    *img_height = img->h;
    *tile_count_x = img->w / IMG_PIXEL_SIZE;
    *tile_count_y = img->h / IMG_PIXEL_SIZE;

    SDL_FreeSurface(img);
    return sprite;
}

Image *imageAlloc()
{
    if (img_pool.head != MAX_IMAGES)
    {
        return &img_pool.images[img_pool.head++];
    }
    if (img_pool.free_images)
    {
        Image *output = img_pool.free_images;
        img_pool.free_images = img_pool.free_images->next;
        return output;
    }

    verify(false, "no more images available, please tell chiefkat to increase the limit or implement an image system that doesn't suck", __LINE__);
    return NULL;
}
void imageLoadSurfaceImage(Image *img, const char *path)
{
    img->img = loadSurfaceImage(path, &img->xcount, &img->ycount, &img->width, &img->height);
    // img->clip_rect = (SDL_Rect){0, 0, IMG_PIXEL_SIZE, IMG_PIXEL_SIZE};
    img->clip_rect = (SDL_Rect){0, 0, IMG_PIXEL_SIZE, IMG_PIXEL_SIZE};
}
Image *imageLoad(const char *path)
{
    Image *img = imageAlloc();
    imageLoadSurfaceImage(img, path);
    return img;
}
void imageDraw(Image *img, int x, int y, int tx, int ty)
{
    SDL_Rect scale_rect = {x * IMG_PIXEL_SIZE, y * IMG_PIXEL_SIZE, IMG_PIXEL_SIZE, IMG_PIXEL_SIZE};
    img->clip_rect = (SDL_Rect){.x = tx * IMG_PIXEL_SIZE, .y = ty * IMG_PIXEL_SIZE, .w = IMG_PIXEL_SIZE, .h = IMG_PIXEL_SIZE};
    SDL_RenderCopy(app.renderer, img->img, &img->clip_rect, &scale_rect);
}
void imageFree(Image *img)
{
    img->next = img_pool.free_images;
    img_pool.free_images = img;
}