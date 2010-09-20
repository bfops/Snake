#include "screen.h"

#include <SDL/SDL.h>
#include <stdio.h>

static const char* windowTitle = "ReWritable's Snake";
static Dimensions windowSize = {
	.width = 800,
	.height = 600
};

static int fatally_die(const char* message)
{
	printf("ERROR: %s\n", message);
	return 1;
}

int main()
{
	if(SDL_Init(SDL_INIT_VIDEO))
		return fatally_die("Could not initialize SDL.");

	atexit(SDL_Quit);

	SDL_Surface* window = make_window(windowTitle, windowSize, false);

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	printf("%p\n", window);

	return 0;
}
