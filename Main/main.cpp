#include <boost/array.hpp>
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

typedef array<Wall, 4> Walls;

static Walls make_walls(Point screenBounds);

namespace {
Logger::Handle logger = Logger::RequestHandle("main()");
}

// TODO: fetch this dynamically
const unsigned int FPS = 60;

// TODO: use more interrupts rather than loops
static void main_loop(Screen& screen, Snake& player, const Walls& walls, bool& quit)
{
	while(!quit && !player.IsDead())
	{
		SDL_PollEvent(NULL);
		player.Update();

		World::Update(screen);

		this_thread::sleep(posix_time::millisec(1000 / FPS));
	}
	logger.Debug("You dead");
}

// TODO: pause functionality
// TODO: have a start screen
// TODO: procedurally-generated adventure mode
int main()
{
	SDLInitializer sdl_keepalive;

	const char* windowTitle = "Rewritable's Snake";
	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_SetEventFilter(Event::Handler);
	SDL_ShowCursor(SDL_DISABLE);

	Screen screen(800, 600);
	Walls walls = make_walls(screen.GetBounds());

	logger.Debug("Creating player");
	Snake player(screen.GetCenter());
	bool quit = false;

	Event::RegisterPlayer(player);
	Event::RegisterQuitter(quit);

	while(!quit)
	{
		main_loop(screen, player, walls, quit);
		player.Reset(screen.GetCenter());
	}

	return 0;
}

static boost::array<Wall, 4> make_walls(Point screenBounds)
{
	const unsigned int wallThickness = 10;
	boost::array<Wall, 4> walls;

	walls[0] = Wall(Point(0, 0), wallThickness, screenBounds.y);
	walls[1] = Wall(Point(screenBounds.x - wallThickness, 0), wallThickness, screenBounds.y);
	walls[2] = Wall(Point(0, 0), screenBounds.x, wallThickness);
	walls[3] = Wall(Point(0, screenBounds.y - wallThickness), screenBounds.x, wallThickness);

	for(Walls::iterator i = walls.begin(), end = walls.end(); i != end; ++i)
		i->AddToWorld();

	return walls;
}
