#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#include <stdlib.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_mixer.h>
#include "./sdlsrc/include/SDL2/SDL.h"
#include "./sdlsrc/include/SDL2/SDL_image.h"
#include "./sdlsrc/include/SDL2/SDL_mixer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef uint8_t byte;
typedef uint16_t two_bytes;
typedef uint32_t four_bytes;
typedef uint64_t eight_bytes;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

#define Log(s, i) printf("\tLogging message: \"[%s] with parameter [%i]\"\n", s, i);
#define Logerror(s, i) printf("\tLogging error message: \"error message [%s], returned from line %i\"\n", s, i);

static inline void verify(bool check, const char *message, int line_number)
{
	if (check)
		return;

	char *buf = (char *)calloc(1, strlen(message) + sizeof(int));
	sprintf(buf, "%s, line %i\n", message, line_number);
	Logerror(message, line_number);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Crasssssshed :(", buf, NULL);
	exit(1);
}
static inline void talkative_verify(bool check, const char *message, int line_number)
{
	if (check)
	{
		Log("verify call success", check);
		return;
	}

	Logerror(message, line_number);
	exit(1);
}

static uint32 current_time, prev_time, frame_time, accumulated_time;

// browser runs slower for some reason. Maybe look into it?
#ifdef __EMSCRIPTEN__
static uint32 tick_speed = 60;
#endif
#ifndef __EMSCRIPTEN__
static uint32 tick_speed = 2;
#endif

#endif
