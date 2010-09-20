#include "graphics.h"

#include <stddef.h>

#include <GL/gl.h>

#include <SDL/SDL.h>

bool graphics_init(const char* windowTitle, Dimensions screenSize, bool fullScreen, Color clearColor)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	// 24-bits of RGB, 8 bits of alpha
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	unsigned flags =
		  SDL_ANYFORMAT
		| SDL_HWSURFACE | SDL_OPENGL
		| (fullScreen ? SDL_FULLSCREEN : 0)
	;

	bool ret = SDL_SetVideoMode(screenSize.width, screenSize.height, 0, flags) != NULL;

	// These values are just the default. The main program can just re-call
	// these functions with different values if it really wants to.
	glViewport(0, 0, screenSize.width, screenSize.height);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	return ret;
}

void graphics_tick(DrawFunc draw, void* param)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw(param);
	SDL_GL_SwapBuffers();
}

void graphics_shutdown()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
