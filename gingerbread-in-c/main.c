#include "graphics.h"

#include <SDL/SDL.h>
#include <stdio.h>

static const char* windowTitle = "ReWritable's Snake";

static Dimensions windowSize = {
	.width = 800,
	.height = 600
};

static Color backgroundColor = {
	.r = 0,
	.g = 0,
	.b = 255,
	.a = 0
};

static int fatally_die(const char* message)
{
	printf("ERROR: %s\n", message);
	exit(1);
}

static void init_sdl()
{
	if(SDL_Init(0))
		fatally_die("Could not initialize SDL.");
	atexit(SDL_Quit);
}

static void init_graphics()
{
	if(!graphics_init(windowTitle, windowSize, false, backgroundColor))
		fatally_die("Could not initialize the graphics subsystem.");
	atexit(graphics_shutdown);
}

int main()
{
	init_sdl();
	init_graphics();

	return 0;
}
