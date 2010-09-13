#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Logger.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "SDL.h"
#include "Snake.hpp"
#include "Wall.hpp"
#include "World.hpp"

using namespace boost;
using namespace std;

typedef array<Wall, 4> WallBox;

static WallBox make_walls(Point maxScreenPoint);
static void add_walls_to_world(WallBox& walls);

namespace {
Logger::Handle logger = Logger::RequestHandle("main()");
DEF_CONSTANT(const char*, windowTitle, "Rewritable's Snake")
DEF_CONSTANT(unsigned int, wallThickness, 10)
// TODO: fetch FPS dynamically
DEF_CONSTANT(unsigned int, FPS, 60)
}

/// Returns true if we should continue playing, false otherwise.
static bool main_loop(Snake& player)
{
	bool quit = false;
	Event::RegisterQuitter(quit);

	while(!player.IsDead())
	{
		if(quit)
			return false;

		SDL_PollEvent(NULL);
		player.Update();

		World::Update();

		this_thread::sleep(posix_time::millisec(1000 / FPS()));
	}

	logger.Debug("You dead");
	return true;
}

int main()
{
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle(), windowTitle());
	SDL_SetEventFilter(Event::Handler);
	SDL_ShowCursor(SDL_DISABLE);

	WallBox walls = make_walls(World::GetBounds().max);

	Point screenCenter = World::GetCenter();
	logger.Debug("Creating player");
	Snake player(screenCenter);

	Event::RegisterPlayer(player);

	while(main_loop(player))
	{
		World::Reset();
		player.Reset(screenCenter);
		add_walls_to_world(walls);
	}

	return 0;
}

static WallBox make_walls(Point screenBounds)
{
	WallBox walls;

	walls[0] = Wall(Point(0, 0), wallThickness(), screenBounds.y);
	walls[1] = Wall(Point(screenBounds.x - wallThickness(), 0), wallThickness(), screenBounds.y);
	walls[2] = Wall(Point(0, 0), screenBounds.x, wallThickness());
	walls[3] = Wall(Point(0, screenBounds.y - wallThickness()), screenBounds.x, wallThickness());

	add_walls_to_world(walls);

	return walls;
}
static void add_walls_to_world(WallBox& walls)
{
	for_each(walls.begin(), walls.end(), bind(&Wall::AddToWorld, _1));
}
