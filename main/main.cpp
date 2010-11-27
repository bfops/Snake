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


/// return false to kill the game loop, true otherwise
typedef bool (GameLoop)(Screen& renderTarget, Timer& gameTimer);
static GameLoop default_game_loop;
static GameLoop quit_game_loop;
static GameLoop* currentGameLoop;

/// return true to play again, false otherwise
typedef bool (ReplayLoop)(Screen& renderTarget, Timer& gameTimer);
static ReplayLoop default_replay_loop;
static ReplayLoop quit_replay_loop;
static ReplayLoop* currentReplayLoop;

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

int main(int, char*[])
{
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	Screen screen(800, 600);
	Timer timer;
	gameObjects = boost::shared_ptr<ZippedUniqueObjectList>(new ZippedUniqueObjectList());
	gameWorld = boost::shared_ptr<GameWorld>(new GameWorld(*gameObjects));

	EventHandler::GetCurrentEventHandler() = &defaultEventHandler;
	currentWorldUpdater = &default_world_updater;
	currentGameLoop = &default_game_loop;
	currentReplayLoop = &default_replay_loop;

	Mix_AllocateChannels(4);

#ifdef MUSIC
	Music music("resources/title theme.wav");
#endif

	while(currentReplayLoop(screen, timer))
		SDL_Delay(1);

	return 0;
}

static bool default_game_loop(Screen& screen, Timer& timer)
{
	Graphics::Update(gameObjects->graphics, screen);
	currentWorldUpdater(*gameWorld, timer);
	EventHandler::GetCurrentEventHandler()->HandleEventQueue(*gameObjects);

	return true;
}

static bool quit_game_loop(Screen&, Timer&)
{
	DEBUGLOG(logger, "Quit called")
	return false;
}

static bool lost_game_loop(Screen&, Timer&)
{
	DEBUGLOG(logger, "DEATH")
	return false;
}

static bool default_replay_loop(Screen& screen, Timer& timer)
{
	while(currentGameLoop(screen, timer))
		SDL_Delay(1000 / FPS);

	gameWorld->Reset(*gameObjects);
	timer.Reset();
	currentGameLoop = &default_game_loop;

	return true;
}

static bool quit_replay_loop(Screen&, Timer&)
{
	return false;
}

static void quit_handler()
{
	currentGameLoop = &quit_game_loop;
	currentReplayLoop = &quit_replay_loop;
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
