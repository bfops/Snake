#include "event.h"
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
	if(message)
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

static void init_event()
{
	if(!event_init())
		fatally_die("Could not initialize the event subsystem.");
	atexit(event_shutdown);
}

static void draw_square(void* x)
{
	graphics_set_camera(NULL);
}

int main()
{
	init_sdl();
	init_graphics();
	init_event();

	EventHandler quitHandler = {
		.type = EVENT_QUIT,
		.handler = (void (*)(void*))fatally_die,
		.param = NULL
	};

	event_add_handler(quitHandler);

	for(;;)
	{
		event_tick();
		graphics_tick((DrawFunc)graphics_set_camera, NULL);
	}

	return 0;
}
