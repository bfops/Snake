#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <SDL/SDL.h>

#include "Screen.hpp"
#include "Snake.hpp"

using namespace boost;
using namespace std;

static bool successful_sdl_setup();

int main()
{
	// TODO: throw exception instead of return value
	if(!successful_sdl_setup())
	{
		cout << SDL_GetError() << "\n";

		exit(0);
	}

	// TODO: wrap SDL_Surface* in custom class (will also handle errors in Surface allocation)
	Screen screen(800, 600);
	Snake player;

	// game loop
	for(;;)
	{
		// TODO: interrupt handle
		player.GetInput();
		// TODO: pass deltaT
		player.Update();
		player.Draw(screen);

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
