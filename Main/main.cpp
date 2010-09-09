#include <ctime>
#include <cstdio>

#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "DebugLogger.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "Physics.hpp"

#include "Wall.hpp"

using namespace boost;
using namespace std;

// TODO: fetch this dynamically
const unsigned int FPS = 60;

// TODO: pause functionality
int main()
{
	// TODO: start screen
	// TODO: procedurally-generated adventure mode

	const char windowTitle = "Rewritable's Snake";
	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_SetEventFilter(Event::Handler);
	SDL_ShowCursor(SDL_DISABLE);

	Point screenBounds(800, 600);
	Screen screen(screenBounds.x, screenBounds.y);

	bool quit = false;
	Snake player(screen.GetCenter());

	Event::RegisterPlayer(player);
	Event::RegisterQuitter(quit);

	// TODO: abstract wall init code out
	const unsigned int wallThickness = 10;
	Wall walls[] = {
		Wall(Point(0, 0), wallThickness, screenBounds.y),
		Wall(Point(screenBounds.x - wallThickness, 0), wallThickness, screenBounds.y),
		Wall(Point(0, 0), screenBounds.x, wallThickness),
		Wall(Point(0, screenBounds.y - wallThickness), screenBounds.x, wallThickness)
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

		DebugLogger::Log("Updating player...\n");
		player.Update();
		DebugLogger::Log("Done updating player\n");

		DebugLogger::Log("Updating PhysicsWorld...\n");
		PhysicsWorld::Update();
		DebugLogger::Log("Done updating PhysicsWorld\n");

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
