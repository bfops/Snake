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

static inline Point get_world_center()
{
	return Point(Config::Get().worldBounds.max.x / 2,
	             Config::Get().worldBounds.max.y / 2);
}

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

static inline void add_sound(GameWorld::SoundList& sounds, const std::string& filename)
{
	sounds.push_back(Sound(filename));
}

static inline void play_spawn_sound(GameWorld::SoundList& sounds)
{
	add_sound(sounds, Config::Get().resources.spawn);
}

// checks if _probability_ occurred in _randnum_ probability-checking can be done by seeing if
// _randnum_ < _probability_ * _max_number_. However, this means if we check for 1/6,
// and then check for 1/3, since (_max_)(1/6) is encompassed in (_max_)(1/3), this can lead
// to unexpected results. Therefore, the region used in calculation is subtracted from _randnum_,
// so that it may be called again without having to account for these side-effects. (if the probability
// was hit, we can assume they won't be checking for more probabilities)
static bool probability_hit(unsigned int& randnum, const double probability, const unsigned int randMax)
{
	const unsigned int border = intRound(randMax * probability);
	if(randnum < border)
		return true;

	randnum -= border;
	return false;
}

static const Config::SpawnData::FoodData& get_food_type()
{
	minstd_rand0 rand(time(NULL));

	// food appearance rates can't have a higher resolution than 1 / randMax
	const unsigned int randMax = 1000;
	unsigned int randnum = rand() % (randMax + 1);

	const Config::SpawnData::Menu& foods = Config::Get().spawn.foodsData;

	// TODO: fix the case where size == 0
	for(Config::SpawnData::Menu::const_iterator i = foods.begin(), end = foods.end() - 1; i != end; ++i)
		if(probability_hit(randnum, i->rate, randMax))
			return *i;

	// TODO: only spawn if a rate is hit
	return foods.back();
}

static Sentinel get_new_sentinel(const unsigned long sentinelSize, const Bounds& worldBounds)
{
	minstd_rand0 rand(time(NULL));

	// get  random number between the worldBounds
#define GETSIZEDRANDOM(m) (rand() % ((worldBounds.max.m - worldBounds.min.m) - sentinelSize + 1) + worldBounds.min.m)
	Point location(GETSIZEDRANDOM(x), GETSIZEDRANDOM(y));
#undef GETSIZEDRANDOM

	return Sentinel(location, sentinelSize);
}

void remove_ptr_from_game_objects(const GameWorld::SpawnPtr& ptr, ZippedUniqueObjectList& gameObjects)
{
	gameObjects.Remove(*ptr);
}

static inline Spawn* make_spawn(const Sentinel& sentinel, const unsigned int spawnSize)
{
	if(Config::Get().survival)
		return new Mine(sentinel, spawnSize);
	else
		return new Food(sentinel, spawnSize, get_food_type());
}

void GameWorld::SpawnLoop()
{
	Timer spawnTimer;

	while(!reset)
	{
		if(spawnTimer.ResetIfHasElapsed(Config::Get().spawn.period))
		{
			Sentinel sentinel(get_new_sentinel(Config::Get().spawn.sentinelSize, Config::Get().worldBounds));
			while(Physics::AnyCollide(sentinel, gameObjects.physics))
			{
				sentinel = get_new_sentinel(Config::Get().spawn.sentinelSize, Config::Get().worldBounds);
				SDL_Delay(10);
			}

			DOLOCKEDZ(gameObjects,
				DOLOCKED(spawnMutex,
					spawns.push_back(GameWorld::SpawnPtr(make_spawn(sentinel, Config::Get().spawn.size)));
					gameObjects.Add(**spawns.rbegin());
				)
			)

			play_spawn_sound(sounds);
			Logger::Debug("Spawn");
		}

		SDL_Delay(100);
	}
	
	DOLOCKEDZ(gameObjects,
		DOLOCKED(spawnMutex,
			for_each(spawns.begin(), spawns.end(), boost::bind(&remove_ptr_from_game_objects, _1, boost::ref(gameObjects)));
		)
	)
}

void GameWorld::Init()
{
	reset = false;
	spawnThread = thread(boost::bind(&GameWorld::SpawnLoop, this));
}

GameWorld::GameWorld(ZippedUniqueObjectList& _gameObjects) :
	gameObjects(_gameObjects), player(get_world_center(), gameObjects)
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
	player.Reset(get_world_center(), gameObjects);

	// wait for everything to finish
	spawnThread.join();

	spawns.clear();
	sounds.clear();

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

static inline void play_death_sound(GameWorld::SoundList& sounds)
{
	add_sound(sounds, Config::Get().resources.die);
}

static inline void play_eat_sound(GameWorld::SoundList& sounds)
{
	add_sound(sounds, Config::Get().resources.eat);
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
				EventHandler::Get()->LossCallback();
			)
			play_death_sound(sounds);
		}
		else if(collisionType & WorldObject::food)
		{
			DOLOCKED(spawnMutex,
				Food* const toRemove = reinterpret_cast<Food*>((o1.GetObjectType() == WorldObject::food) ? &o1 : &o2);
				for(SpawnList::iterator i = spawns.begin(), end = spawns.end(); i != end; ++i)
				{
					if(&**i == toRemove)
					{
						DOLOCKEDZ(gameObjects,
							gameObjects.Remove(**i);
						)
						spawns.erase(i);
						break;
					}
				}
			)
			play_eat_sound(sounds);
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
