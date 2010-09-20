#pragma once
#include <stdbool.h>

/// Represents the dimensions of the screen.
typedef struct
{
	int width;
	int height;
} Dimensions;

typedef struct
{
	float r;
	float g;
	float b;
	float a;
} Color;

/** Initializes the graphics subsystem. Unless this is called, graphics API
    calls may not be used.

    @param windowTitle  The title that will be seen in the taskbar as well as
                        the titlebar in windowed mode.
    @param screenSize   The dimensions of the screen we will create.
    @param fullScreen   Will the window be created as fullscreen or windowed?
    @param clearColor   The color the screen will be cleared to every frame.

    @return True if successful, false otherwise.
*/
bool graphics_init(
	const char* windowTitle,
	Dimensions screenSize,
	bool fullScreen,
	Color clearColor
);

/// A function callback to handle the drawing of the scene.
typedef void (*DrawFunc)(void* param);

/**
	Draws one frame of the scene.

	@param draw  A functor, taking a void*. `param` will be passed to it. All
	             opengl drawing must be done in this function.
	@param param The parameter to pass to draw when ready.
*/
void graphics_tick(DrawFunc draw, void* param);

/// Gracefully exits the graphics subsystem.
void graphics_shutdown();
