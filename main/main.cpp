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
#pragma warning(push, 0)
#endif

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <SDL.h>
#include <SDL_mixer.h>

#ifdef MSVC
#pragma warning(pop)
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
static void physics_loop();

static const EventHandler defaultEventHandler(
	quit_handler, loss_handler, default_pause_handler,
	default_key_handler, default_mouse_handler);

static const EventHandler pausedEventHandler(
	quit_handler, loss_handler, paused_pause_handler,
	paused_key_handler, paused_mouse_handler);

bool quit, lost, paused;

int main(int, char*[])
{
	quit = lost = paused = false;
	// keep SDL active as long as this is in scope
	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	gameObjects = boost::shared_ptr<ZippedUniqueObjectList>(new ZippedUniqueObjectList());
	gameWorld = boost::shared_ptr<GameWorld>(new GameWorld(*gameObjects));

	DOLOCKED(EventHandler::mutex,
		EventHandler::Get() = &defaultEventHandler;
	)

	// TODO: knock this back down once "not enough channels" bug is fixed
	Mix_AllocateChannels(100);

#ifdef MUSIC
	Music music("resources/title theme.wav");
#endif
	
	thread graphicsThread(graphics_loop);
	thread physicsThread(physics_loop);

	while(!quit)
	{
		// TODO: replace "lost" with a [mutex + interrupt]
		while(!lost && !quit)
		{
			if(!paused)
				gameWorld->Update();

			DOLOCKED(EventHandler::Get()->mutex,
				EventHandler::Get()->HandleEventQueue();
			)
			SDL_Delay(1000 / FPS);
		}
		if(lost)
		{
			DEBUGLOG(logger, "DEATH")
			gameWorld->Reset();
		}
		lost = false;
	}
	DEBUGLOG(logger, "Quit called")

	// wait for everything to complete
	graphicsThread.join();
	physicsThread.join();

	return 0;
}

static void graphics_loop()
{
	Screen screen(800, 600);

	while(!quit)
	{
		Graphics::Update(gameObjects->graphics, screen);
		SDL_Delay(1000 / FPS);
	}
}

static void physics_loop()
{
	while(!quit)
	{
		Physics::Update(*gameWorld, gameObjects->physics);
		SDL_Delay(5);
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
	DOLOCKED(EventHandler::mutex,
		EventHandler::Get() = &pausedEventHandler;
	)
	paused = true;
	Clock::Get().Pause();
}

static void paused_pause_handler()
{
	DOLOCKED(EventHandler::mutex,
		EventHandler::Get() = &defaultEventHandler;
	)
	paused = false;
	Clock::Get().Unpause();
}

static void default_key_handler(const SDLKey key)
{
	gameWorld->KeyNotify(key);
}

static void paused_key_handler(const SDLKey) {}

static void default_mouse_handler(const Uint8 button)
{
	gameWorld->MouseNotify(button);
}

static void paused_mouse_handler(const Uint8) {}
