#include <ctime>

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
	// initialize random seed
	srand(time(NULL));

	// TODO: throw exception instead of return value
	if(!successful_sdl_setup())
	{
		cout << SDL_GetError() << "\n";

		exit(0);
	}

	Screen screen(810, 600);
	Snake player;
	player.SetRenderTarget(screen);
	player.Center();

	// TODO: use more interrupts rather than loops
	// TODO: use timer rather than loop cycles
	// TODO: pass deltaT to update functions

	// game loop
	while(!quit_called())
	{
		screen.Clear();
		player.GetInput();
		player.Update();
		player.Draw();

		screen.Update();

		if(player.IsDead())
		{
			cout << "You lose!\n";
			break;
		}

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
