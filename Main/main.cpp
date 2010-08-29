#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <SDL/SDL.h>

#include "Screen.hpp"
#include "Snake.hpp"

using namespace boost;
using namespace std;

static bool successful_sdl_setup();
static bool quit_called();

int main()
{
	// TODO: throw exception instead of return value
	if(!successful_sdl_setup())
	{
		cout << SDL_GetError() << "\n";

		exit(0);
	}

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

		// TODO: pass deltaT
		screen.Update();

		if(player.IsDead() || quit_called())
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
bool quit_called()
{
	SDL_Event event;
	return (SDL_PollEvent(&event) != 0 && event.type == SDL_QUIT);
}
