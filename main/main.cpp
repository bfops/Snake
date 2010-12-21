#include "Clock.hpp"
#include "Common.hpp"
#include "Config.hpp"
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
#include <list>
#include <SDL.h>

#ifdef MSVC
#pragma warning(pop)
#endif

using namespace boost;
using namespace std;

static EventHandler::QuitCallbackType quit_handler;
static EventHandler::LossCallbackType loss_handler;
static EventHandler::PauseCallbackType default_pause_handler;
static EventHandler::PauseCallbackType paused_pause_handler;
static EventHandler::SoundCallbackType sound_handler;
static EventHandler::KeyCallbackType default_key_handler;
static EventHandler::KeyCallbackType paused_key_handler;
static EventHandler::MouseCallbackType default_mouse_handler;
static EventHandler::MouseCallbackType paused_mouse_handler;

static const char* windowTitle("ReWritable's Snake");
static boost::shared_ptr<ZippedUniqueObjectList> gameObjects;
static boost::shared_ptr<GameWorld> gameWorld;

typedef std::list<const std::string> SoundQueue;
static Mutex soundMutex;
static SoundQueue soundQueue;

// returns false iff loading failed
static void physics_loop();
static void game_loop();

static const EventHandler defaultEventHandler(quit_handler, loss_handler, default_pause_handler,
	sound_handler, default_key_handler, default_mouse_handler);

static const EventHandler pausedEventHandler(quit_handler, loss_handler, paused_pause_handler,
	sound_handler, paused_key_handler, paused_mouse_handler);

bool quit, lost, paused;

int main(int, char*[])
{
	typedef std::list<Sound> SoundList;
	SoundList sounds;
	quit = lost = paused = false;

	SDLInitializer keepSDLInitialized;

	SDL_WM_SetCaption(windowTitle, windowTitle);
	SDL_ShowCursor(SDL_DISABLE);

	gameObjects = boost::shared_ptr<ZippedUniqueObjectList>(new ZippedUniqueObjectList());
	gameWorld = boost::shared_ptr<GameWorld>(new GameWorld(*gameObjects));

	DOLOCKED(EventHandler::mutex,
		EventHandler::Get() = &defaultEventHandler;
	)

	Mix_AllocateChannels(10);

	const Music music(Config::Get().resources.theme);
	
	Timer screenUpdate;
	const Screen screen(Config::Get().screen.w, Config::Get().screen.h);

	thread physicsThread(physics_loop);
	thread gameThread(game_loop);

	while(!quit)
	{
		if(screenUpdate.ResetIfHasElapsed(1000 / Config::Get().FPS))
		{
			DOLOCKED(gameObjects->graphics.mutex,
				Graphics::Update(gameObjects->graphics, screen);
			)
		}

		DOLOCKED(soundMutex,
			if(soundQueue.size() > 0)
			{
				// TODO: clear finished sounds
				sounds.push_back(Sound(soundQueue.front()));
				soundQueue.pop_front();
			}
		)

		DOLOCKED(EventHandler::mutex,
			EventHandler::Get()->HandleEventQueue();
		)
	}

	// wait for everything to complete
	physicsThread.join();
	gameThread.join();

	sounds.clear();

	return 0;
}

static void physics_loop()
{
	while(!quit)
	{
		DOLOCKED(gameObjects->physics.mutex,
			Physics::Update(*gameWorld, gameObjects->physics);
		)
		SDL_Delay(5);
	}
}

static void game_loop()
{
	while(!quit)
	{
		while(!lost && !quit)
		{
			if(!paused)
				gameWorld->Update();

			SDL_Delay(5);
		}
		if(lost)
		{
			Logger::Debug("DEATH");
			gameWorld->Reset();
			lost = false;
		}
	}
	Logger::Debug("Quit called");
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

static void sound_handler(const std::string& filename)
{
	DOLOCKED(soundMutex,
		soundQueue.push_back(filename);
	)
}

static void default_key_handler(const SDLKey key)
{
	gameWorld->KeyNotify(key);
}

static void paused_key_handler(const SDLKey)
{
}

static void default_mouse_handler(const Uint8 button)
{
	gameWorld->MouseNotify(button);
}

static void paused_mouse_handler(const Uint8)
{
}
