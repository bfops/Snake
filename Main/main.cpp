#include <boost/date_time.hpp>
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
#include "ZippedUniqueObjectList.hpp"

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static const char* windowTitle("ReWritable's Snake");
static const unsigned int FPS(60);

static inline posix_time::ptime get_current_time()
{
	return posix_time::microsec_clock::local_time();
}

/// Returns true if we should continue playing, false otherwise.
static inline bool main_loop(GameWorld& gameWorld, ZippedUniqueObjectList& gameObjects,
	EventHandler& eventHandler, Screen& screen, boost::posix_time::ptime& before)
{
	while(!gameWorld.Lost() && !gameWorld.QuitCalled())
	{
		Graphics::Update(gameObjects.graphics, screen);
		Physics::Update(gameObjects.physics);

		const unsigned int elapsedTime = (get_current_time() - before).total_milliseconds();
		gameWorld.Update(gameObjects, elapsedTime);
		before = get_current_time();

		eventHandler.Update(gameWorld, gameObjects);

		this_thread::sleep(posix_time::millisec(1000 / FPS));
	}
	if(gameWorld.QuitCalled())
	{
		DEBUGLOG(logger, "Quit called")
		return false;
	}

	DEBUGLOG(logger, "Death")
	return true;
}

int main()
{
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	ZippedUniqueObjectList gameObjects;
	Screen screen(800, 600);
	EventHandler eventHandler;
	boost::posix_time::ptime currentTime = get_current_time();
	GameWorld gameWorld(gameObjects);

	while(main_loop(gameWorld, gameObjects, eventHandler, screen, currentTime))
	{
		gameWorld.Reset(gameObjects);
	}

	return 0;
}
