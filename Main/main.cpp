#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "GameWorld.hpp"
#include "Logger.hpp"
#include "SDL.h"

using namespace boost;
using namespace std;

namespace {
Logger::Handle logger(Logger::RequestHandle("main()"));

DEF_CONSTANT(const char*, windowTitle, "Rewritable's Snake")
// TODO: fetch FPS dynamically
DEF_CONSTANT(unsigned int, FPS, 60)
}

/// Returns true if we should continue playing, false otherwise.
static bool main_loop(GameWorld& gameWorld, UniqueObjectList& gameObjects)
{
	while(!gameWorld.Lost() && !gameWorld.QuitCalled())
	{
		gameWorld.Update(gameObjects);

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

	UniqueObjectList gameObjects;
	GameWorld gameWorld(gameObjects);

	while(main_loop(gameWorld, gameObjects))
	{
		gameWorld.Reset(gameObjects);
	}

	return 0;
}
