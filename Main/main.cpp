#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Logger.hpp"
#include "Event.hpp"
#include "SDL.h"
#include "Snake.hpp"
#include "World.hpp"

using namespace boost;
using namespace std;

namespace {
Logger::Handle logger = Logger::RequestHandle("main()");
DEF_CONSTANT(const char*, windowTitle, "Rewritable's Snake")
// TODO: fetch FPS dynamically
DEF_CONSTANT(unsigned int, FPS, 60)
}

/// Returns true if we should continue playing, false otherwise.
static bool main_loop(World& world, Snake& player)
{
	bool quit = false;
	Event::RegisterQuitter(quit);

	while(!player.IsDead())
	{
		if(quit)
			return false;

		SDL_PollEvent(NULL);

		player.Update();
		world.Update();

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

	// TODO: move this entire block of stuff
	// into main_loop (it need not be in a
	// higher scope)
	World world;
	logger.Debug("Creating player");
	Snake player(world);

	Event::RegisterPlayer(player);
	Event::RegisterWorld(world);

	while(main_loop(world, player))
	{
		world.Reset();
		player.Reset(world);
	}

	return 0;
}
