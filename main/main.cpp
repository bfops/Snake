#include <SDL.h>
#include <SDL_mixer.h>

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
#include <boost/concept_check.hpp>

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("main()"));

static const char* windowTitle("ReWritable's Snake");
static const unsigned int FPS(60);

/// Returns true if we should continue playing, false otherwise.
static inline bool game_loop(GameWorld& gameWorld, ZippedUniqueObjectList& gameObjects,
	Screen& screen, GameState gameState)
{
	while(!gameWorld.Lost() && !gameState.QuitCalled())
	{
		gameState.Update();
		Graphics::Update(gameObjects.graphics, screen);

		if(!gameState.IsPaused())
		{
			Physics::Update(gameWorld, gameObjects.physics);

			gameWorld.Update(gameObjects, gameState.GetElapsedTime());
		}

		EventHandler::HandleEventQueue(gameState, gameObjects);

		SDL_Delay(1000 / FPS);
	}
	if(gameState.QuitCalled())
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
	GameWorld gameWorld(gameObjects);

	Mix_AllocateChannels(3);

	Mix_Music* music = Mix_LoadMUS("resources/title theme.waV");
	Mix_PlayMusic(music, -1);

	while(game_loop(gameWorld, gameObjects, screen, GameState(gameWorld)))
		gameWorld.Reset(gameObjects);

	Mix_FreeMusic(music);

	return 0;
}
