#pragma once
#include <SDL/SDL.h>

#include <stdbool.h>

typedef struct
{
	int width;
	int height;
} Dimensions;

SDL_Surface* make_window(const char* title, Dimensions dims, bool fullScreen);
