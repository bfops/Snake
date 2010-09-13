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

namespace {
Logger::Handle logger = Logger::RequestHandle("main()");
DEF_CONSTANT(const char*, windowTitle, "Rewritable's Snake")
DEF_CONSTANT(unsigned int, wallThickness, 10)
// TODO: fetch FPS dynamically
DEF_CONSTANT(unsigned int, FPS, 60)
}

/// Returns true if we should continue playing, false otherwise.
static bool main_loop(Screen& screen, Snake& player, const WallBox& walls)
{
	bool quit = false;
	Event::RegisterQuitter(quit);

	while(!player.IsDead())
	{
		if(quit)
			return false;

		SDL_PollEvent(NULL);
		player.Update();

		World::Update(screen);

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

	Screen screen(800, 600);
	WallBox walls = make_walls(screen.GetBounds());

	logger.Debug("Creating player");
	Snake player(screen.GetCenter());

	Event::RegisterPlayer(player);

	while(main_loop(screen, player, walls))
	{
		player.Reset(screen.GetCenter());
	}

	return 0;
}

static boost::array<Wall, 4> make_walls(Point screenBounds)
{
	boost::array<Wall, 4> walls;

	walls[0] = Wall(Point(0, 0), wallThickness(), screenBounds.y);
	walls[1] = Wall(Point(screenBounds.x - wallThickness(), 0), wallThickness(), screenBounds.y);
	walls[2] = Wall(Point(0, 0), screenBounds.x, wallThickness());
	walls[3] = Wall(Point(0, screenBounds.y - wallThickness()), screenBounds.x, wallThickness());

	for_each(walls.begin(), walls.end(), bind(&Wall::AddToWorld, _1));

	return walls;
}
