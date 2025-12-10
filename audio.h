#ifndef AUDIO_H
#define AUDIO_H

#include "definitions.h"

typedef struct _Music
{
    int volume;
    double fade_time;
    Mix_Chunk *snd;
    int fading;
    struct _Music *next;
} Music;

#define MAX_MUSIC 4
typedef struct
{
    Music music[MAX_MUSIC];
    Music *free_music;
    unsigned int head;
} MusicPool;
extern MusicPool music_pool;

Music *musicAlloc();
void loadMusicMixLoadWAV(Music *mus, const char *path);
Music *musicLoad(const char *path);
void musicPlay(Music *mus, double loopstart);
void musicFree(Music *mus);
void musicFadeIn(Music *mus);
void musicFadeOut(Music *mus);

#endif