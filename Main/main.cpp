#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <SDL/SDL.h>

#include "Snake.hpp"

using namespace boost;
using namespace std;

static bool successful_sdl_setup();
static SDL_Surface* make_window();

int main()
{
	// TODO: throw exception instead of return value
	if(!successful_sdl_setup())
	{
		cout << SDL_GetError() << "\n";

		exit(0);
	}

	// TODO: custom window class
	SDL_Surface* window = make_window();

	Snake player;
	// game loop
	for(;;)
	{
		// TODO: interrupt handle
		player.GetInput();
		// TODO: pass deltaT
		player.Update();
		player.Draw(window);

		if(player.IsDead())
			break;

		this_thread::sleep(posix_time::millisec(50));
	}

	exit(0);
}

bool successful_sdl_setup()
{
	atexit(SDL_Quit);

	const unsigned int SDLSubsystems = SDL_INIT_VIDEO;
	return (SDL_Init(SDLSubsystems) == 0);
}
SDL_Surface* make_window()
{
	return SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
}
