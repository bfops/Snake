#include "GameWorld.hpp"

#include "Common.hpp"
#include "Config.hpp"
#include "custom_algorithm.hpp"
#include "EventHandler.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Mine.hpp"
#include "Physics.hpp"
#include "Wall.hpp"
#include "ZippedUniqueObjectCollection.hpp"

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

static void make_new_wall(GameWorld::WallCollection& walls, const Config::WallConfig& wallConfig)
{
	walls.push_back(Wall(wallConfig.bounds, wallConfig.color));
}

static inline void make_walls(GameWorld::WallCollection& walls)
{
	for_each(Config::Get().wallsConfig.begin(), Config::Get().wallsConfig.end(),
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

void remove_ptr_from_game_objects(const GameWorld::SpawnPtr& ptr, ZippedUniqueObjectCollection& gameObjects)
{
	gameObjects.Remove(*ptr);
}

void remove_pair_from_game_objects(const GameWorld::FunctionalSpawnCollection::value_type& pair,
	ZippedUniqueObjectCollection& gameObjects)
{
	for_each(pair.second.begin(), pair.second.end(),
		boost::bind(&remove_ptr_from_game_objects, _1, boost::ref(gameObjects)));
}

static GameWorld::SpawnPtr get_new_spawn(const Config::SpawnCollectionConfig::SpawnConfig& spawnConfig)
{
	const Bounds& spawnBounds = Config::Get().spawns.bounds;
	boost::minstd_rand rand(time(NULL));

	// get random number between the worldBounds
#define GETSIZEDRANDOM(m) (rand() % ( \
	(spawnBounds.max.m - spawnBounds.min.m) - (spawnConfig.size + spawnConfig.cushion) + 1) + spawnBounds.min.m)

	Point location(GETSIZEDRANDOM(x), GETSIZEDRANDOM(y));
#undef GETSIZEDRANDOM

	return spawnConfig.ConstructSpawn(location);
}

static inline const Config::SpawnCollectionConfig::SpawnConfig* get_spawn_data()
{
	boost::minstd_rand rand(time(NULL));

	// food appearance rates can't have a higher resolution than 1 / randMax
	const unsigned long randMax = 1000;
	unsigned int randnum = rand() % (randMax + 1);

	const Config::SpawnCollectionConfig::SpawnCollection& spawnsConfig = Config::Get().spawns.spawnsConfig;
	for(Config::SpawnCollectionConfig::SpawnCollection::const_iterator i = spawnsConfig.begin(), end = spawnsConfig.end();
		i != end; ++i)
		if(probability_hit(randnum, (*i)->rate, randMax))
			return &**i;

	return NULL;
}

void GameWorld::SpawnLoop()
{
	Timer spawnTimer;

	while(!reset)
	{
		if(spawnTimer.ResetIfHasElapsed(Config::Get().spawns.period))
		{
			const Config::SpawnCollectionConfig::SpawnConfig* const spawnConfig = get_spawn_data();
			if(spawnConfig)
			{
				SpawnPtr spawn;
				do
				{
					spawn = get_new_spawn(*spawnConfig);
					SDL_Delay(10);
				}
				while(Physics::AnyCollide(*spawn, gameObjects.physics));

				spawn->ShrinkDown(spawnConfig->size);

				// TODO: remove collided spawns
				DOLOCKEDZ(gameObjects,
					DOLOCKED(spawnMutex,
						const Clock::TimeType expiryTime = Clock::Get().GetTime() + spawnConfig->expiry;
						SpawnCollection& equalSpawns = spawns[expiryTime];
						equalSpawns.push_back(SpawnPtr(spawn));
						gameObjects.Add(*equalSpawns.back());

						play_spawn_sound();
						Logger::Debug("Spawn");
					)
				)
			}
		}

		// if the first set of spawns in the map is expired, remove the set
		DOLOCKED(spawnMutex,
			if(spawns.size() > 0)
			{
				const FunctionalSpawnCollection::value_type& firstSet = *spawns.begin();

				if(firstSet.first < Clock::Get().GetTime())
				{
					DOLOCKEDZ(gameObjects,
						remove_pair_from_game_objects(firstSet, gameObjects);
					)
					spawns.erase(firstSet.first);
				}
			}
		)

		SDL_Delay(100);
	}
	
	DOLOCKEDZ(gameObjects,
		DOLOCKED(spawnMutex,
			for_each(spawns.begin(), spawns.end(),
				boost::bind(&remove_pair_from_game_objects, _1, boost::ref(gameObjects)));
		)
	)
}

void GameWorld::Init()
{
	reset = false;
	spawnThread = boost::thread(boost::bind(&GameWorld::SpawnLoop, this));
}

GameWorld::GameWorld(ZippedUniqueObjectCollection& _gameObjects) :
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
