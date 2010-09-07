#include <ctime>
#include <cstdio>

#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "Physics.hpp"

#include "Wall.hpp"

using namespace boost;
using namespace std;

// TODO: fetch this dynamically
const unsigned int FPS = 60;

int main()
{
	// TODO: make event filters work!
	// they only seem to get called when SDL_PollEvent
	// is called, which makes it an exercise in futility
	// because then all events are removed from te queue anyway
	SDL_SetEventFilter(Event::Handler);
	SDL_ShowCursor(SDL_DISABLE);

	Screen screen(810, 600, 54, 40);
	Point screenBounds = screen.GetBlockBounds();

	bool quit = false;
	Snake player(screen.GetCenter());

	Event::RegisterPlayer(player);
	Event::RegisterQuitter(quit);

	// TODO: abstract this block out
	Wall walls[] = {
		Wall(Point(0, 0), 1, screenBounds.y),
		Wall(Point(screenBounds.x - 1, 0), 1, screenBounds.y),
		Wall(Point(0, 0), screenBounds.x, 1),
		Wall(Point(0, screenBounds.y - 1), screenBounds.x, 1)
	};
	for(unsigned int i = 0; i < countof(walls); ++i)
	{
		PhysicsWorld::AddObject(walls[i]);
	}

	// TODO: use more interrupts rather than loops
	// game loop
	while(!quit)
	{
		SDL_PollEvent(NULL);
		player.Update();
		PhysicsWorld::Update();

		screen.Clear();
		for(unsigned int i = 0; i < countof(walls); ++i)
			screen.Draw(walls[i], walls[i].color);
		player.Draw(screen);
		screen.Update();

		if(player.IsDead())
		{
			printf("You lose!\n");
			break;
		}

		this_thread::sleep(posix_time::millisec(1000 / FPS));
	}

	return 0;
}
