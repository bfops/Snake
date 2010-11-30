#include "Clock.hpp"
#include "Common.hpp"
#include "EventHandler.hpp"
#include "GameWorld.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include "Music.hpp"
#include "Physics.hpp"
#include "Screen.hpp"
#include "SDLInitializer.hpp"
#include "Timer.hpp"
#include "ZippedUniqueObjectList.hpp"

#ifdef MSVC
#pragma warning( push, 0 )
#endif

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <SDL.h>
#include <SDL_mixer.h>

#ifdef MSVC
#pragma warning( pop )
#endif

// TODO: make different game modes call
// different functions, not use scattered
// preprocessor macros

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static EventHandler::QuitCallbackType quit_handler;
static EventHandler::LossCallbackType loss_handler;
static EventHandler::PauseCallbackType default_pause_handler;
static EventHandler::PauseCallbackType paused_pause_handler;
static EventHandler::KeyCallbackType default_key_handler;
static EventHandler::KeyCallbackType paused_key_handler;
static EventHandler::MouseCallbackType default_mouse_handler;
static EventHandler::MouseCallbackType paused_mouse_handler;

static const char* windowTitle("ReWritable's Snake");
// TODO: change this back to 60 once multithreading kicks in,
// but right now, movement is a little jerky at high speed
static const unsigned int FPS(120);
static boost::shared_ptr<ZippedUniqueObjectList> gameObjects;
static boost::shared_ptr<GameWorld> gameWorld;

static void graphics_loop();

static const EventHandler defaultEventHandler(
	quit_handler, loss_handler, default_pause_handler,
	default_key_handler, default_mouse_handler);

static const EventHandler pausedEventHandler(
	quit_handler, loss_handler, paused_pause_handler,
	paused_key_handler, paused_mouse_handler);

typedef void (WorldUpdater)(GameWorld&);
static WorldUpdater default_world_updater;
static WorldUpdater paused_world_updater;
static WorldUpdater* currentWorldUpdater;

bool quit, lost;

int main(int, char*[])
{
	quit = false;
	lost = false;
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	gameObjects = boost::shared_ptr<ZippedUniqueObjectList>(new ZippedUniqueObjectList());
	gameWorld = boost::shared_ptr<GameWorld>(new GameWorld(*gameObjects));

	EventHandler::GetCurrentEventHandler() = &defaultEventHandler;
	currentWorldUpdater = &default_world_updater;

	Mix_AllocateChannels(5);

#ifdef MUSIC
	Music music("resources/title theme.wav");
#endif
	
	thread graphicsThread(graphics_loop);
	while(!quit)
	{
		while(!lost && !quit)
		{
			currentWorldUpdater(*gameWorld);
			EventHandler::GetCurrentEventHandler()->HandleEventQueue();
			SDL_Delay(1000 / FPS);
		}
		if(lost)
		{
			DEBUGLOG(logger, "DEATH")
			gameWorld->Reset(*gameObjects);
		}
		lost = false;
	}
	DEBUGLOG(logger, "Quit called")

	// wait for everything to complete
	graphicsThread.join();

	return 0;
}

static void graphics_loop()
{
	Screen screen(800, 600);

	while(!quit)
	{
		gameObjects->graphics.Lock();
		Graphics::Update(gameObjects->graphics, screen);
		gameObjects->graphics.Unlock();
		SDL_Delay(1000 / FPS);
	}
}

static void quit_handler()
{
	quit = true;
}

static void loss_handler()
{
	lost = true;
}

static void default_pause_handler()
{
	EventHandler::GetCurrentEventHandler() = &pausedEventHandler;
	currentWorldUpdater = &paused_world_updater;
	Clock::Get().Pause();
}

static void paused_pause_handler()
{
	EventHandler::GetCurrentEventHandler() = &defaultEventHandler;
	currentWorldUpdater = &default_world_updater;
	Clock::Get().Unpause();
}

static void default_key_handler(const SDLKey key)
{
	gameWorld->KeyNotify(key, *gameObjects);
}

static void paused_key_handler(const SDLKey) {}

static void default_mouse_handler(const Uint8 button)
{
	gameWorld->MouseNotify(button, *gameObjects);
}

static void paused_mouse_handler(const Uint8) {}

static void default_world_updater(GameWorld& world)
{
	Physics::Update(world, *gameObjects);
	world.Update(*gameObjects);
}

static void paused_world_updater(GameWorld&)
{
}
