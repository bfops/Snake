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

static vector<Wall> create_walls(Point screenBounds);

// TODO: fetch this dynamically
const unsigned int FPS = 60;

// TODO: pause functionality
int main()
{
	// TODO: start screen
	// TODO: procedurally-generated adventure mode

	const char* windowTitle = "Rewritable's Snake";
	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_SetEventFilter(Event::Handler);
	SDL_ShowCursor(SDL_DISABLE);

	Screen screen(800, 600);
	vector<Wall> walls = create_walls(screen.GetBounds());

	bool quit = false;
	DebugLogger::Log("Creating player\n");
	DebugLogger::Indent* indent = new DebugLogger::Indent();
	Snake player(screen.GetCenter());
	delete indent;

	Event::RegisterPlayer(player);
	Event::RegisterQuitter(quit);

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
		for(vector<Wall>::iterator wall = walls.begin(), end = walls.end(); wall != end; ++wall)
			wall->Draw(screen);
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

static vector<Wall> create_walls(Point screenBounds)
{
	const unsigned int wallThickness = 10;
	vector<Wall> walls;
	walls.reserve(4);
	walls.push_back(Wall(Point(0, 0), wallThickness, screenBounds.y));
	walls.push_back(Wall(Point(screenBounds.x - wallThickness, 0), wallThickness, screenBounds.y));
	walls.push_back(Wall(Point(0, 0), screenBounds.x, wallThickness));
	walls.push_back(Wall(Point(0, screenBounds.y - wallThickness), screenBounds.x, wallThickness));

	for(vector<Wall>::iterator i = walls.begin(), end = walls.end(); i != end; ++i)
		i->AddToWorld();

	return walls;
}
