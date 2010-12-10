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

// returns false iff loading failed
static bool load_game_config(istream& inputStream);
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
	if(!load_game_config(configFile))
		logger.Fatal("Loading failed!");

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

	Mix_AllocateChannels(100);

	if(Config::Get().music)
		music = new Music(Config::Get().resources.theme);
	
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

static inline std::string get_wall_data_name(const unsigned short i, const char* specifier)
{
	std::stringstream s;
	s << "wall"
	  << i
	  << specifier;

	return s.str();
}
// TODO: place this config-loading responsibility inside ConfigLoader
static bool load_game_config(istream& inputStream)
{
	bool success = true;
	// TODO: fail on [errors in loading]
	Config& config = Config::Get();
	ConfigLoader in(inputStream);

	success &= in.Pop("survival", config.survival);
	success &= in.Pop("music", config.music);
	success &= in.Pop("FPS", config.FPS);

	success &= in.Pop("resourceEat", config.resources.eat);
	success &= in.Pop("resourceSpawn", config.resources.spawn);
	success &= in.Pop("resourceDie", config.resources.die);
	success &= in.Pop("resourceIntro", config.resources.gameIntro);
	success &= in.Pop("resourceTheme", config.resources.theme);
	success &= in.Pop("resourceStartup", config.resources.startup);

	bool wallLoaded = true;
	for(unsigned short i = 0; wallLoaded; ++i)
	{
		unsigned int x, y, w, h;
		wallLoaded &= in.Pop(get_wall_data_name(i, "MinX"), x);
		wallLoaded &= in.Pop(get_wall_data_name(i, "MinY"), y);
		wallLoaded &= in.Pop(get_wall_data_name(i, "MaxX"), w);
		wallLoaded &= in.Pop(get_wall_data_name(i, "MaxY"), h);

		if(wallLoaded)
		{
			w -= x;
			h -= y;

			const Config::WallData newWall = {x, y, w, h};;
			config.wallData.push_back(newWall);
		}
	}

	success &= in.Pop("worldBoundsMinX", config.worldBounds.min.x);
	success &= in.Pop("worldBoundsMinY", config.worldBounds.min.y);
	success &= in.Pop("worldBoundsMaxX", config.worldBounds.max.x);
	success &= in.Pop("worldBoundsMaxY", config.worldBounds.max.y);

	if(config.survival)
	{
		success &= in.Pop("mineAdditionPeriod", config.spawnPeriod);
		success &= in.Pop("mineSize", config.spawnSize);
		success &= in.Pop("mineSentinelSize", config.sentinelSize);

		success &= in.Pop("survivalPointGainPeriod", config.pointGainPeriod);
		success &= in.Pop("survivalPointGainAmount", config.pointGainAmount);
		success &= in.Pop("survivalSnakeSpeedupPeriod", config.snake.speedupPeriod);
	}
	else
	{
		success &= in.Pop("foodAdditionPeriod", config.spawnPeriod);
		success &= in.Pop("foodSize", config.spawnSize);
		success &= in.Pop("foodSentinelSize", config.sentinelSize);

		success &= in.Pop("normalPointGainPeriod", config.pointGainPeriod);
		success &= in.Pop("normalPointGainAmount", config.pointGainAmount);
		success &= in.Pop("normalSnakeSpeedupPeriod", config.snake.speedupPeriod);
	}

	success &= in.Pop("snakeDefaultLength", config.snake.startingLength);
	success &= in.Pop("snakeWidth", config.snake.width);
	success &= in.Pop("snakeDefaultSpeed", config.snake.startingSpeed);
	success &= in.Pop("snakeSpeedupAmount", config.snake.speedupAmount);
	success &= in.Pop("snakeGrowthCap", config.snake.growthCap);
	success &= in.Pop("snakeGrowthRate", config.snake.growthRate);

	return success;
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
