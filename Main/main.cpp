#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "EventHandler.hpp"
#include "GameWorld.hpp"
#include "Logger.hpp"
#include "SDL.h"

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static const char* windowTitle("ReWritable's Snake");
static const unsigned int FPS(60);

/// Returns true if we should continue playing, false otherwise.
static inline bool main_loop(GameWorld& gameWorld, UniqueObjectList& gameObjects, EventHandler& eventHandler)
{
	while(!gameWorld.Lost() && !gameWorld.QuitCalled())
	{
		gameWorld.Update(gameObjects);
		eventHandler.Update(gameWorld, gameObjects);

		this_thread::sleep(posix_time::millisec(1000 / FPS));
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

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	UniqueObjectList gameObjects;
	EventHandler eventHandler;
	GameWorld gameWorld(gameObjects);

	while(main_loop(gameWorld, gameObjects, eventHandler))
	{
		gameWorld.Reset(gameObjects);
	}

	return 0;
}
