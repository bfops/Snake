#include "GameWorld.hpp"

#include "Common.hpp"
#include "custom_algorithm.hpp"
#include "EventHandler.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Mine.hpp"
#include "Physics.hpp"
#include "Wall.hpp"
#include "ZippedUniqueObjectList.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/bind.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include <functional>
#include <SDL_timer.h>
#include <SDL_mixer.h>

#ifdef MSVC
#pragma warning(pop)
#endif

using namespace boost;

static Logger::Handle logger(Logger::RequestHandle("GameWorld"));

// GAMECONSTANT: food management
static const unsigned int mineAdditionPeriod(3000);
static const unsigned int mineSize(10);
static const unsigned int mineSentinelSize(20);
static const unsigned int foodAdditionPeriod(8000);
static const unsigned int foodSize(15);
static const unsigned int foodSentinelSize(17);

static const unsigned int wallThickness(10);
static const Bounds worldBounds(Point(0, 0), Point(800, 600));

static inline void make_walls(GameWorld::WallBox& walls)
{
	walls.push_back(Wall(Point(0, 0), wallThickness, worldBounds.max.y));
	walls.push_back(Wall(Point(worldBounds.max.x - wallThickness, 0), wallThickness, worldBounds.max.y));
	walls.push_back(Wall(Point(0, 0), worldBounds.max.x, wallThickness));
	walls.push_back(Wall(Point(0, worldBounds.max.y - wallThickness), worldBounds.max.x, wallThickness));
#ifdef COOLERWALLS
	walls.push_back(Wall(Point(worldBounds.max.x / 2, 0), wallThickness, worldBounds.max.y / 4));
	walls.push_back(Wall(Point(worldBounds.max.x / 2, worldBounds.max.y * 3 / 4), wallThickness, worldBounds.max.y / 4));
	walls.push_back(Wall(Point(0, worldBounds.max.y / 2), worldBounds.max.x / 4, wallThickness));
	walls.push_back(Wall(Point(worldBounds.max.x * 3 / 4, worldBounds.max.y / 2), worldBounds.max.x / 4, wallThickness));
#endif
}

static void sound_playing_thread(const char* const filename)
{
	Mix_Chunk* const sound = Mix_LoadWAV(filename);
	int channel;
	if(sound == NULL || (channel = Mix_PlayChannel(-1, sound, 0)) == -1)
	{
		logger.Fatal(format("Error playing \"%1%\": %2%") % filename % Mix_GetError());
		return;
	}

	while(Mix_Playing(channel))
		SDL_Delay(1);

	Mix_HaltChannel(channel);
	Mix_FreeChunk(sound);
}

static inline void play_sound(const char* filename)
{
	thread(sound_playing_thread, filename);
}

static inline void play_food_sound()
{
	play_sound("resources/food appear.wav");
}

static inline void play_mine_sound()
{
	play_sound("resources/mine appear.wav");
}

/// checks if _probability_ occurred in _randnum_
/// probability-checking can be done by seeing if
/// _randnum_ < _probability_ * _max_number_.
/// However, this means if we check for 1/6,
/// and then check for 1/3, since (_max_)(1/6)
/// is encompassed in (_max_)(1/3), this can lead
/// to unexpected results. Therefore, the region
/// used in calculation is subtracted from _randnum_,
/// so that it may be called again without having to
/// account for these side-effects. (if the probability
/// was hit, we can assume they won't be checking for
/// more probabilities)
static bool probability_hit(unsigned int& randnum, double probability, unsigned int randMax)
{
	unsigned int border = round(randMax * probability);
	if(randnum < border)
		return true;

	randnum -= border;
	return false;
}

static Food::FoodInfo get_food_type()
{
	minstd_rand0 rand(time(NULL));

	const unsigned int randMax = 100;
	unsigned int randnum = rand() % (randMax + 1);

	if(probability_hit(randnum, 1.0 / 20.0, randMax))
		return Food::ice;

	if(probability_hit(randnum, 1.0 / 10.0, randMax))
		return Food::celery;

	if(probability_hit(randnum, 1.0 / 5.0, randMax))
		return Food::donut;

	return Food::normal;
}

static Sentinel get_new_sentinel(unsigned long sentinelSize)
{
	minstd_rand0 rand(time(NULL));

#define GETSIZEDRANDOM(m) (rand() % ((worldBounds.max.m - worldBounds.min.m) - sentinelSize + 1) + worldBounds.min.m)
	Point location(GETSIZEDRANDOM(x), GETSIZEDRANDOM(y));
#undef GETSIZEDRANDOM

	return Sentinel(location, sentinelSize);
}

// TODO: factor [food & mine] loops together
void GameWorld::FoodLoop()
{
	Timer foodTimer;

	while(!reset)
	{
		while(foodTimer.ResetIfHasElapsed(foodAdditionPeriod))
		{
			Sentinel sentinel(get_new_sentinel(foodSentinelSize));
			while(Physics::AnyCollide(sentinel, gameObjects.physics))
			{
				sentinel = get_new_sentinel(foodSentinelSize);
				SDL_Delay(10);
			}

			DOLOCKEDZ(gameObjects,
				DOLOCKED(foodMutex,
					gameObjects.RemoveRange(foods.begin(), foods.end());
					foods.push_back(Food(sentinel, foodSize, get_food_type()));
					gameObjects.AddRange(foods.begin(), foods.end());
				)
			)

			play_food_sound();
			logger.Debug("Food spawn");
		}

		SDL_Delay(100);
	}
	
	DOLOCKEDZ(gameObjects,
		DOLOCKED(foodMutex,
			gameObjects.RemoveRange(foods.begin(), foods.end());
		)
	)
}

void GameWorld::MineLoop()
{
	Timer mineTimer;
	MineList mines;

	while(!reset)
	{
		while(mineTimer.ResetIfHasElapsed(mineAdditionPeriod))
		{
			Sentinel sentinel(get_new_sentinel(mineSentinelSize));
			while(Physics::AnyCollide(sentinel, gameObjects.physics))
			{
				sentinel = get_new_sentinel(mineSentinelSize);
				SDL_Delay(10);
			}

			DOLOCKEDZ(gameObjects,
				gameObjects.RemoveRange(mines.begin(), mines.end());
				mines.push_back(Mine(sentinel, mineSize));
				gameObjects.AddRange(mines.begin(), mines.end());
			)

			play_mine_sound();
			logger.Debug("Mine spawn");
		}

		SDL_Delay(100);
	}

	DOLOCKEDZ(gameObjects,
		gameObjects.RemoveRange(mines.begin(), mines.end());
	)
}

void GameWorld::Init()
{
	reset = false;

#ifdef SURVIVAL
	spawnThread = thread(boost::bind(&GameWorld::MineLoop, this));
#else
	spawnThread = thread(boost::bind(&GameWorld::FoodLoop, this));
#endif
}

GameWorld::GameWorld(ZippedUniqueObjectList& _gameObjects) :
	gameObjects(_gameObjects), player(GetCenter(), gameObjects)
{
	make_walls(walls);
	DOLOCKEDZ(gameObjects,
		gameObjects.AddRange(walls.begin(), walls.end());
	)
	
	Init();
}

void GameWorld::Update()
{
	player.Update(gameObjects);
}

void GameWorld::Reset()
{
	reset = true;
	player.Reset(GetCenter(), gameObjects);

	// wait for everything to finish
	spawnThread.join();

	foods.clear();

	Init();
}

static Direction get_direction_from_key(const SDLKey key)
{
	switch(key)
	{
		case SDLK_LEFT:
			return Direction::left;

		case SDLK_RIGHT:
			return Direction::right;

		case SDLK_UP:
			return Direction::up;

		case SDLK_DOWN:
			return Direction::down;

		default:
			logger.Fatal("Invalid key somehow passed to get_direction_from_key");
			return Direction::empty;
	}
}

Direction get_direction_from_button(const Uint8 button)
{
	switch(button)
	{
		case SDL_BUTTON_LEFT:
			return Direction::left;

		case SDL_BUTTON_RIGHT:
			return Direction::right;

		default:
			return Direction::empty;
	}
}

static inline void play_death_sound()
{
	play_sound("resources/die.wav");
}

static inline void play_eat_sound()
{
	play_sound("resources/eat.wav");
}

void GameWorld::CollisionHandler(WorldObject& o1, WorldObject& o2)
{
	o1.CollisionHandler(o2);
	o2.CollisionHandler(o1);

	const unsigned int collisionType = o1.GetObjectType() | o2.GetObjectType();
	const bool selfCollide = !(collisionType & ~o1.GetObjectType());

	if(collisionType & WorldObject::snake)
	{
		if(selfCollide || collisionType & WorldObject::wall	|| collisionType & WorldObject::mine)
		{
			DOLOCKED(EventHandler::mutex,
				EventHandler::Get()->LossNotify();
			)
			play_death_sound();
		}
		else if(collisionType & WorldObject::food)
		{
			DOLOCKED(foodMutex,
				DOLOCKEDZ(gameObjects,
					gameObjects.RemoveRange(foods.begin(), foods.end());
				)
				Food* const toRemove = reinterpret_cast<Food*>((o1.GetObjectType() == WorldObject::food) ? &o1 : &o2);
				for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
				{
					if(&*i == toRemove)
					{
						foods.erase(i);
						break;
					}
				}
				DOLOCKEDZ(gameObjects,
					gameObjects.AddRange(foods.begin(), foods.end());
				)
			)
			play_eat_sound();
		}
	}
}

void GameWorld::KeyNotify(const SDLKey key)
{
	if(key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN)
		player.ChangeDirection(get_direction_from_key(key), gameObjects);
}

void GameWorld::MouseNotify(Uint8 button)
{
	if(button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT)
		player.Turn(get_direction_from_button(button), gameObjects);
}

Point GameWorld::GetCenter() const
{
	return Point(worldBounds.max.x / 2, worldBounds.max.y / 2);
}
