#include "Clock.hpp"
#include "Common.hpp"
#include "Config.hpp"
#include "ConfigLoader.hpp"
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
static boost::shared_ptr<ZippedUniqueObjectList> gameObjects;
static boost::shared_ptr<GameWorld> gameWorld;

static void load_game_config(istream& inputStream);
static void physics_loop();
static void game_loop();

static const EventHandler defaultEventHandler(
	quit_handler, loss_handler, default_pause_handler,
	default_key_handler, default_mouse_handler);

static const EventHandler pausedEventHandler(
	quit_handler, loss_handler, paused_pause_handler,
	paused_key_handler, paused_mouse_handler);

bool quit, lost, paused;

int main(int, char*[])
{
	Music* music = NULL;
	std::ifstream configFile("game.cfg");
	load_game_config(configFile);
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

	if(Config::Get().music)
		music = new Music("resources/title theme.wav");
	
	Timer screenUpdate;
	Screen screen(800, 600);

	thread physicsThread(physics_loop);
	thread gameThread(game_loop);

	while(!quit)
	{
		if(screenUpdate.ResetIfHasElapsed(1000 / Config::Get().FPS))
			Graphics::Update(gameObjects->graphics, screen);

		DOLOCKED(EventHandler::Get()->mutex,
			EventHandler::Get()->HandleEventQueue();
		)
	}

	// wait for everything to complete
	physicsThread.join();
	gameThread.join();

	if(music)
		delete music;

	return 0;
}

// TODO: place this config-loading responsibility inside ConfigLoader
static void load_game_config(istream& inputStream)
{
	Config& config = Config::Get();
	const ConfigLoader in(inputStream);

	in.Get("survival", config.survival);
	in.Get("music", config.music);
	in.Get("FPS", config.FPS);
	
	in.Get("wallThickness", config.wallThickness);
	in.Get("worldBoundsMinX", config.worldBounds.min.x);
	in.Get("worldBoundsMinY", config.worldBounds.min.y);
	in.Get("worldBoundsMaxX", config.worldBounds.max.x);
	in.Get("worldBoundsMaxY", config.worldBounds.max.y);

	if(config.survival)
	{
		in.Get("mineAdditionPeriod", config.spawnPeriod);
		in.Get("mineSize", config.spawnSize);
		in.Get("mineSentinelSize", config.sentinelSize);

		in.Get("survivalPointGainPeriod", config.pointGainPeriod);
		in.Get("survivalPointGainAmount", config.pointGainAmount);
		in.Get("survivalSnakeSpeedupPeriod", config.snake.speedupPeriod);
	}
	else
	{
		in.Get("foodAdditionPeriod", config.spawnPeriod);
		in.Get("foodSize", config.spawnSize);
		in.Get("foodSentinelSize", config.sentinelSize);

		in.Get("normalPointGainPeriod", config.pointGainPeriod);
		in.Get("normalPointGainAmount", config.pointGainAmount);
		in.Get("normalSnakeSpeedupPeriod", config.snake.speedupPeriod);
	}

	in.Get("snakeDefaultLength", config.snake.startingLength);
	in.Get("snakeWidth", config.snake.width);
	in.Get("snakeDefaultSpeed", config.snake.startingSpeed);
	in.Get("snakeSpeedupAmount", config.snake.speedupAmount);
	in.Get("snakeGrowthCap", config.snake.growthCap);
	in.Get("snakeGrowthRate", config.snake.growthRate);
}

static void physics_loop()
{
	while(!quit)
	{
		Physics::Update(*gameWorld, gameObjects->physics);
		SDL_Delay(5);
	}
}

static void game_loop()
{
	while(!quit)
	{
		// TODO: replace _lost_ with a [mutex + interrupt]
		while(!lost && !quit)
		{
			if(!paused)
				gameWorld->Update();

			SDL_Delay(5);
		}
		if(lost)
		{
			DEBUGLOG(logger, "DEATH")
			gameWorld->Reset();
		}
		lost = false;
	}
	DEBUGLOG(logger, "Quit called")
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
