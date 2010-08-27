#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <SDL/SDL.h>

using namespace boost;

static void MakeWindow();

int main()
{
	MakeWindow();
	SDL_Delay(2000);

	SDL_Quit();
	return 0;
}

void MakeWindow()
{
	SDL_Surface* screen;
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1024, 768, 32, SDL_SWSURFACE);
	SDL_Flip(screen);
}
