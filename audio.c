#include "audio.h"

Music *musicAlloc()
{
    if (music_pool.head != MAX_MUSIC)
    {
        return &music_pool.music[music_pool.head++];
    }
    if (music_pool.free_music)
    {
        Music *out = music_pool.free_music;
        music_pool.free_music = music_pool.free_music->next;
        return out;
    }

    verify(false, "Ran out of music to play. Please tell chief to increase the limit or implement a better music system.", __LINE__);
    return NULL;
}
void loadMusicMixLoadWAV(Music *mus, const char *path)
{
    mus->snd = Mix_LoadWAV(path);
    verify(mus, "failed to load music", __LINE__);
}
Music *musicLoad(const char *path)
{
    Music *mus = musicAlloc();
    loadMusicMixLoadWAV(mus, path);
    return mus;
}
void musicPlay(Music *mus)
{
    Mix_PlayChannel(-1, mus->snd, -1);
    Mix_VolumeChunk(mus->snd, 0);
}
void musicFree(Music *mus)
{
    mus->next = music_pool.free_music;
    music_pool.free_music = mus;
}
void musicFadeIn(Music *mus)
{
    if (mus->fading)
    {
        if (mus->fade_time >= MIX_MAX_VOLUME)
        {
            mus->fading = 0;
        }
        else
        {
            mus->fade_time += 0.01 * (double)tick_speed;
        }
        Mix_VolumeChunk(mus->snd, (int)mus->fade_time);
        return;
    }
    mus->fading = 1;
    mus->fade_time = Mix_VolumeChunk(mus->snd, -1);
}
void musicFadeOut(Music *mus)
{
    if (mus->fading)
    {
        if (mus->fade_time <= 0.0)
        {
            mus->fading = 0;
        }
        else
        {
            mus->fade_time -= 0.01 * (double)tick_speed;
        }
        Mix_VolumeChunk(mus->snd, (int)mus->fade_time);
        return;
    }
    mus->fading = 1;
    mus->fade_time = Mix_VolumeChunk(mus->snd, -1);
}