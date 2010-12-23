#include "GameWorld.hpp"

#include "Common.hpp"
#include "Config.hpp"
#include "custom_algorithm.hpp"
#include "EventHandler.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Mine.hpp"
#include "Physics.hpp"
#include "Sentinel.hpp"
#include "Wall.hpp"
#include "ZippedUniqueObjectList.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/bind.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <functional>
#include <SDL_timer.h>
#include <SDL_mixer.h>
#include <string>

#ifdef MSVC
#pragma warning(pop)
#endif

using boost::format;
using boost::thread;
using boost::minstd_rand0;

static void make_new_wall(GameWorld::WallList& walls, const Config::WallsData::WallData& wallData)
{
	const Point lowerBound(wallData.x, wallData.y);
	const Wall newWall(lowerBound, wallData.w, wallData.h);
	walls.push_back(newWall);
}

static inline void make_walls(GameWorld::WallList& walls)
{
	walls.clear();

	for_each(Config::Get().wallsData.wallsData.begin(), Config::Get().wallsData.wallsData.end(),
		boost::bind(&make_new_wall, boost::ref(walls), _1));
}

static inline void play_sound(const std::string& filename)
{
	EventHandler::Get()->SoundCallback(filename);
}

static inline void play_spawn_sound()
{
	play_sound(Config::Get().resources.spawn);
}

static inline void play_death_sound()
{
	play_sound(Config::Get().resources.die);
}

static inline void play_eat_sound()
{
	play_sound(Config::Get().resources.eat);
}

// checks if _probability_ occurred in _randnum_ probability-checking can be done by seeing if
// _randnum_ <= _probability_ * _max_number_. However, this means if we check for 1/6, and then check for
// 1/3, since (_max_)(1/6) is encompassed in (_max_)(1/3), this can lead to unexpected results. Therefore,
// the region used in calculation is subtracted from _randnum_, so that it may be called again without
// having to account for these side-effects. (if the probability was hit, we can assume they won't be
// checking for more probabilities)
static bool probability_hit(unsigned int& randnum, const double probability, const unsigned long randMax)
{
	const unsigned long border = intRound(randMax * probability);
	if(randnum <= border)
		return true;

	randnum -= border;
	return false;
}

static const Config::SpawnsData::FoodData* get_food_type()
{
	minstd_rand0 rand(time(NULL));

	// food appearance rates can't have a higher resolution than 1 / randMax
	const unsigned long randMax = 1000;
	unsigned int randnum = rand() % (randMax + 1);

	const Config::SpawnsData::Menu& foods = Config::Get().spawns.foodsData;

	for(Config::SpawnsData::Menu::const_iterator i = foods.begin(), end = foods.end(); i != end; ++i)
		if(probability_hit(randnum, i->rate, randMax))
			return &*i;

	return NULL;
}

static Sentinel get_new_sentinel(const unsigned long sentinelSize, const Bounds& bounds)
{
	minstd_rand0 rand(time(NULL));

	// get random number between the worldBounds
#define GETSIZEDRANDOM(m) (rand() % ((bounds.max.m - bounds.min.m) - sentinelSize + 1) + bounds.min.m)
	Point location(GETSIZEDRANDOM(x), GETSIZEDRANDOM(y));
#undef GETSIZEDRANDOM

	return Sentinel(location, sentinelSize);
}

void remove_ptr_from_game_objects(const GameWorld::SpawnPtr& ptr, ZippedUniqueObjectList& gameObjects)
{
	gameObjects.Remove(*ptr);
}

static inline Spawn* make_spawn(const Sentinel& sentinel)
{
	if(Config::Get().survival)
		return new Mine(sentinel);
	else
	{
		const Config::SpawnsData::FoodData* const foodData = get_food_type();

		if(!foodData)
			return NULL;

		return new Food(sentinel, *foodData);
	}
}

void GameWorld::SpawnLoop()
{
	Timer spawnTimer;

	while(!reset)
	{
		if(spawnTimer.ResetIfHasElapsed(Config::Get().spawns.period))
		{
			Sentinel sentinel(get_new_sentinel(Config::Get().spawns.sentinelSize,
				Config::Get().worldBounds));
			while(Physics::AnyCollide(sentinel, gameObjects.physics))
			{
				sentinel = get_new_sentinel(Config::Get().spawns.sentinelSize, Config::Get().worldBounds);
				SDL_Delay(10);
			}
			
			// TODO: remove collided spawns
			DOLOCKEDZ(gameObjects,
				DOLOCKED(spawnMutex,
					Spawn* const spawn = make_spawn(sentinel);
					if(spawn)
					{
						spawns.push_back(SpawnPtr(spawn));
						gameObjects.Add(*spawns.back());

						play_spawn_sound();
						Logger::Debug("Spawn");
					}
				)
			)
		}

		SDL_Delay(100);
	}
	
	DOLOCKEDZ(gameObjects,
		DOLOCKED(spawnMutex,
			for_each(spawns.begin(), spawns.end(),
				boost::bind(&remove_ptr_from_game_objects, _1, boost::ref(gameObjects)));
		)
	)
}

void GameWorld::Init()
{
	reset = false;
	spawnThread = thread(boost::bind(&GameWorld::SpawnLoop, this));
}

GameWorld::GameWorld(ZippedUniqueObjectList& _gameObjects) :
	gameObjects(_gameObjects), player(gameObjects)
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
	player.Reset(gameObjects);

	// wait for everything to finish
	spawnThread.join();

	spawns.clear();

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

void GameWorld::CollisionHandler(WorldObject& o1, WorldObject& o2)
{
	DOLOCKED(o2.mutex,
		o1.CollisionHandler(o2);
	)
	DOLOCKED(o1.mutex,
		o2.CollisionHandler(o1);
	)

	const unsigned long collisionType = o1.GetObjectType() | o2.GetObjectType();
	const bool selfCollide = !(collisionType & ~o1.GetObjectType());

	if(collisionType & WorldObject::snake)
	{
		if(selfCollide || collisionType & WorldObject::wall	|| collisionType & WorldObject::mine)
		{
			DOLOCKED(EventHandler::mutex,
				EventHandler::Get()->LossCallback();
			)
			play_death_sound();
		}
		else if(collisionType & WorldObject::food)
		{
			play_eat_sound();
			DOLOCKED(spawnMutex,
				Food* const toRemove = static_cast<Food*>(
					(o1.GetObjectType() == WorldObject::food) ? &o1 : &o2);

				DOLOCKEDZ(gameObjects,
					gameObjects.Remove(*toRemove);
				)
			)
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
