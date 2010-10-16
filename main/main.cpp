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

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static EventHandler::QuitCallbackType quit_handler;
static EventHandler::PauseCallbackType pause_handler;
static EventHandler::KeyCallbackType key_handler;
static EventHandler::MouseCallbackType mouse_handler;

static const char* windowTitle("ReWritable's Snake");
static const unsigned int FPS(60);
static shared_ptr<GameState> gameState;
static shared_ptr<ZippedUniqueObjectList> gameObjects;
static const EventHandler* currentEventHandler;

static const EventHandler defaultEventHandler(quit_handler, pause_handler, key_handler, mouse_handler);
static const EventHandler pausedEventHandler(quit_handler, pause_handler, NULL, NULL);

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

// *sigh* lambdas would make this so much nicer..
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
static void key_handler(const SDLKey key)
{
	gameState->KeyHandler(key, *gameObjects);
}
static void mouse_handler(const Uint8 button)
{
	gameState->MouseHandler(button, *gameObjects);
}
