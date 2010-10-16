#include "Common.hpp"
#include "EventHandler.hpp"
#include "GameState.hpp"
#include "GameWorld.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include "Physics.hpp"
#include "Screen.hpp"
#include "SDLInitializer.hpp"
#include "Timer.hpp"
#include "ZippedUniqueObjectList.hpp"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <SDL.h>
#include <SDL_mixer.h>

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static EventHandler::QuitCallbackType quit_handler;
static EventHandler::PauseCallbackType pause_handler;
static EventHandler::KeyCallbackType key_handler;
static EventHandler::KeyCallbackType paused_key_handler;
static EventHandler::MouseCallbackType mouse_handler;
static EventHandler::MouseCallbackType paused_mouse_handler;

static const char* windowTitle("ReWritable's Snake");
static const unsigned int FPS(60);
static shared_ptr<GameState> gameState;
static shared_ptr<ZippedUniqueObjectList> gameObjects;
static shared_ptr<GameWorld> gameWorld;

static const EventHandler defaultEventHandler(quit_handler, pause_handler, key_handler, mouse_handler);
static const EventHandler pausedEventHandler(quit_handler, pause_handler, paused_key_handler, paused_mouse_handler);
static const EventHandler* currentEventHandler;

typedef void (WorldUpdater)(GameWorld&, Timer& gameTimer);
static WorldUpdater default_world_updater;
static WorldUpdater paused_world_updater;
static WorldUpdater* currentWorldUpdater;

// whether or not music is on
#define MUSIC
/// Returns true if we should continue playing, false otherwise.
static inline bool game_loop(Screen& screen, Timer& timer)
{
	while(!gameWorld->Lost() && !gameState->QuitCalled())
	{
		Graphics::Update(gameObjects->graphics, screen);
		currentWorldUpdater(*gameWorld, timer);
		currentEventHandler->HandleEventQueue(*gameState, *gameObjects);

		SDL_Delay(1000 / FPS);
	}
	if(gameState->QuitCalled())
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

	Screen screen(800, 600);
	Timer timer;
	gameObjects = shared_ptr<ZippedUniqueObjectList>(new ZippedUniqueObjectList());
	gameWorld = shared_ptr<GameWorld>(new GameWorld(*gameObjects));
	gameState = shared_ptr<GameState>(new GameState(*gameWorld));

	currentEventHandler = &defaultEventHandler;
	currentWorldUpdater = &default_world_updater;

	Mix_AllocateChannels(3);

#ifdef MUSIC
	Mix_Music* music = Mix_LoadMUS("resources/title theme.wav");
	if(music == NULL)
		logger.Fatal(format("Error playing music: %1%") % Mix_GetError());
	Mix_PlayMusic(music, -1);
#endif

	while(game_loop(screen, timer))
	{
		gameWorld->Reset(*gameObjects);
		timer.Reset();
	}

#ifdef MUSIC
	Mix_FreeMusic(music);
#endif

	return 0;
}

static void quit_handler()
{
	gameState->QuitHandler();
}

static void pause_handler()
{
	gameState->PauseHandler();

	if(gameState->IsPaused())
	{
		currentEventHandler = &pausedEventHandler;
		currentWorldUpdater = &paused_world_updater;
	}
	else
	{
		currentEventHandler = &defaultEventHandler;
		currentWorldUpdater = &default_world_updater;
	}
}

static void key_handler(const SDLKey key)
{
	gameWorld->KeyNotify(key, *gameObjects);
}

static void paused_key_handler(const SDLKey) {}

static void mouse_handler(const Uint8 button)
{
	gameWorld->MouseNotify(button, *gameObjects);
}

static void paused_mouse_handler(const Uint8 button) {}

static void default_world_updater(GameWorld& world, Timer& timer)
{
	timer.Update();
	Physics::Update(world, gameObjects->physics);
	world.Update(*gameObjects, timer.GetElapsedTime());
}

static void paused_world_updater(GameWorld&, Timer& timer)
{
	timer.SilentUpdate();
}
