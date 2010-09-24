#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "EventHandler.hpp"
#include "GameWorld.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include "Physics.hpp"
#include "Screen.hpp"
#include "SDL.h"
#include "UniqueObjectList.hpp"

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static const char* windowTitle("ReWritable's Snake");
static const unsigned int FPS(60);

/// Returns true if we should continue playing, false otherwise.
static inline bool main_loop(GameWorld& gameWorld, UniqueObjectList& graphicsObjects,
	UniqueObjectList& physicsObjects, EventHandler& eventHandler, Screen& screen)
{
	while(!gameWorld.Lost() && !gameWorld.QuitCalled())
	{
		Graphics::Update(graphicsObjects, screen);
		Physics::Update(physicsObjects);
		gameWorld.Update(graphicsObjects, physicsObjects);
		eventHandler.Update(gameWorld, graphicsObjects, physicsObjects);

		this_thread::sleep(posix_time::millisec(1000 / FPS));
	}
	if(gameWorld.QuitCalled())
		return false;

	DEBUGLOG(logger, "Death")
	return true;
}

int main()
{
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	UniqueObjectList graphicsObjects;
	UniqueObjectList physicsObjects;
	Screen screen(800, 600);
	EventHandler eventHandler;
	GameWorld gameWorld(graphicsObjects, physicsObjects);

	while(main_loop(gameWorld, graphicsObjects, physicsObjects, eventHandler, screen))
	{
		gameWorld.Reset(graphicsObjects, physicsObjects);
	}

	return 0;
}
