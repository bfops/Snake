#include "Common.hpp"
#include "EventHandler.hpp"
#include "GameState.hpp"
#include "GameWorld.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include "Physics.hpp"
#include "Screen.hpp"
#include "SDLInitializer.hpp"
#include "ZippedUniqueObjectList.hpp"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <SDL.h>
#include <SDL_mixer.h>
#include <boost/concept_check.hpp>
#include <boost/concept_check.hpp>
#include <boost/concept_check.hpp>

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static const char* windowTitle("ReWritable's Snake");
static const unsigned int FPS(60);
static shared_ptr<GameState> gameState;
static shared_ptr<ZippedUniqueObjectList> gameObjects;
static EventHandler* currentEventHandler;
static EventHandler defaultEventHandler, pausedEventHandler;

/// Returns true if we should continue playing, false otherwise.
static inline bool game_loop(GameWorld& gameWorld, Screen& screen)
{
	while(!gameWorld.Lost() && !gameState->QuitCalled())
	{
		gameState->Update();
		Graphics::Update(gameObjects->graphics, screen);

		if(!gameState->IsPaused())
		{
			Physics::Update(gameWorld, gameObjects->physics);

			gameWorld.Update(*gameObjects, gameState->GetElapsedTime());
		}

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

static void quit_handler()
{
	gameState->QuitHandler();
}
static void pause_handler()
{
	gameState->PauseHandler();
	if(gameState->IsPaused())
		currentEventHandler = &pausedEventHandler;
	else
		currentEventHandler = &defaultEventHandler;
}
static void normal_key_handler(const SDLKey key)
{
	gameState->KeyHandler(key, *gameObjects);
}
static void paused_key_handler(const SDLKey)
{

}
static void normal_mouse_handler(const Uint8 button)
{
	gameState->MouseHandler(button, *gameObjects);
}
static void paused_mouse_handler(const Uint8 button)
{

}

int main()
{
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	Screen screen(800, 600);
	gameObjects = shared_ptr<ZippedUniqueObjectList>(new ZippedUniqueObjectList());
	GameWorld gameWorld(*gameObjects);
	gameState = shared_ptr<GameState>(new GameState(gameWorld));

	currentEventHandler = &defaultEventHandler;
	defaultEventHandler.RegisterQuitCallback(quit_handler);
	defaultEventHandler.RegisterPauseCallback(pause_handler);
	defaultEventHandler.RegisterKeyCallback(normal_key_handler);
	defaultEventHandler.RegisterMouseCallback(normal_mouse_handler);

	pausedEventHandler = defaultEventHandler;
	pausedEventHandler.RegisterKeyCallback(paused_key_handler);
	pausedEventHandler.RegisterMouseCallback(paused_mouse_handler);

	Mix_AllocateChannels(3);

	/*Mix_Music* music = Mix_LoadMUS("resources/title theme.wav");
	if(music == NULL)
		logger.Fatal(format("Error playing music: %1%") % Mix_GetError());
	Mix_PlayMusic(music, -1);*/

	while(game_loop(gameWorld, screen))
		gameWorld.Reset(*gameObjects);

	//Mix_FreeMusic(music);

	return 0;
}
