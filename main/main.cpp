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

/// return false to kill the game loop, true otherwise
typedef bool (GameLoop)(Timer& gameTimer);
static GameLoop default_game_loop;
static GameLoop lost_game_loop;
static GameLoop* currentGameLoop;

static const EventHandler defaultEventHandler(
	quit_handler, loss_handler, default_pause_handler,
	default_key_handler, default_mouse_handler);

static const EventHandler pausedEventHandler(
	quit_handler, loss_handler, paused_pause_handler,
	paused_key_handler, paused_mouse_handler);

typedef void (WorldUpdater)(GameWorld&, Timer& gameTimer);
static WorldUpdater default_world_updater;
static WorldUpdater paused_world_updater;
static WorldUpdater* currentWorldUpdater;

bool quit;

int main(int, char*[])
{
	quit = false;
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	Timer timer;
	gameObjects = boost::shared_ptr<ZippedUniqueObjectList>(new ZippedUniqueObjectList());
	gameWorld = boost::shared_ptr<GameWorld>(new GameWorld(*gameObjects));

	EventHandler::GetCurrentEventHandler() = &defaultEventHandler;
	currentWorldUpdater = &default_world_updater;
	currentGameLoop = &default_game_loop;

	Mix_AllocateChannels(4);

#ifdef MUSIC
	Music music("resources/title theme.wav");
#endif

	thread graphicsThread(graphics_loop);
	while(!quit)
	{
		while(currentGameLoop(timer))
			SDL_Delay(1000 / FPS);

		gameWorld->Reset(*gameObjects);
		timer.Reset();
		currentGameLoop = &default_game_loop;
	}

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

static bool default_game_loop(Timer& timer)
{
	currentWorldUpdater(*gameWorld, timer);
	EventHandler::GetCurrentEventHandler()->HandleEventQueue();

	return true;
}

static bool lost_game_loop(Timer&)
{
	DEBUGLOG(logger, "DEATH")
	return false;
}

static void quit_handler()
{
	quit = true;
}

static void loss_handler()
{
	currentGameLoop = &lost_game_loop;
}

static void default_pause_handler()
{
	EventHandler::GetCurrentEventHandler() = &pausedEventHandler;
	currentWorldUpdater = &paused_world_updater;
}

static void paused_pause_handler()
{
	EventHandler::GetCurrentEventHandler() = &defaultEventHandler;
	currentWorldUpdater = &default_world_updater;
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

static void default_world_updater(GameWorld& world, Timer& timer)
{
	timer.Update();
	Physics::Update(world, *gameObjects);
	world.Update(*gameObjects, timer.GetElapsedTime());
}

static void paused_world_updater(GameWorld&, Timer& timer)
{
	timer.SilentUpdate();
}
