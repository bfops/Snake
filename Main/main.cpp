#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "GameWorld.hpp"
#include "Logger.hpp"
#include "SDL.h"
#include "ObjectManager.hpp"

using namespace boost;
using namespace std;

namespace {
Logger::Handle logger = Logger::RequestHandle("main()");
DEF_CONSTANT(const char*, windowTitle, "Rewritable's Snake")
// TODO: fetch FPS dynamically
DEF_CONSTANT(unsigned int, FPS, 60)
}

/// Returns true if we should continue playing, false otherwise.
static bool main_loop(GameWorld& gameWorld)
{
	while(!gameWorld.Lost() && !gameWorld.QuitCalled())
	{
		gameWorld.Update();

		this_thread::sleep(posix_time::millisec(1000 / FPS()));
	}
	if(gameWorld.QuitCalled())
		return false;

	logger.Debug("You dead");
	return true;
}

int main()
{
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle(), windowTitle());
	SDL_ShowCursor(SDL_DISABLE);

	// TODO: move this entire block of stuff
	// into main_loop (it need not be in a
	// higher scope)
	GameWorld gameWorld;

	while(main_loop(gameWorld))
	{
		gameWorld.Reset();
	}

	return 0;
}
