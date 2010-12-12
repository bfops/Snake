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

static void make_new_wall(GameWorld::WallBox& walls, const Config::WallData& wallData)
{
	const Point upperLeftBound(wallData.x, wallData.y);
	const Wall newWall(upperLeftBound, wallData.w, wallData.h);
	walls.push_back(newWall);
}

static inline void make_walls(GameWorld::WallBox& walls)
{
	walls.clear();

	for_each(Config::Get().wallData.begin(), Config::Get().wallData.end(),
		boost::bind(&make_new_wall, boost::ref(walls), _1));
}

static void sound_playing_thread(const std::string& filename)
{
	Mix_Chunk* const sound = Mix_LoadWAV(filename.c_str());

	int channel;
	if(sound == NULL || (channel = Mix_PlayChannel(-1, sound, 0)) == -1)
	{
		Logger::Fatal(format("Error playing sound \"%1%\": %2%") % filename.c_str() % Mix_GetError());
		return;
	}

	while(Mix_Playing(channel))
		SDL_Delay(1);

	Mix_HaltChannel(channel);
	Mix_FreeChunk(sound);
}

static inline void play_sound(const std::string& filename)
{
	thread(sound_playing_thread, filename);
}

static inline void play_spawn_sound()
{
	play_sound(Config::Get().resources.spawn);
}

// checks if _probability_ occurred in _randnum_
// probability-checking can be done by seeing if
// _randnum_ < _probability_ * _max_number_.
// However, this means if we check for 1/6,
// and then check for 1/3, since (_max_)(1/6)
// is encompassed in (_max_)(1/3), this can lead
// to unexpected results. Therefore, the region
// used in calculation is subtracted from _randnum_,
// so that it may be called again without having to
// account for these side-effects. (if the probability
// was hit, we can assume they won't be checking for
// more probabilities)
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

static Sentinel get_new_sentinel(unsigned long sentinelSize, const Bounds& worldBounds)
{
	minstd_rand0 rand(time(NULL));

#define GETSIZEDRANDOM(m) (rand() % ((worldBounds.max.m - worldBounds.min.m) - sentinelSize + 1) + worldBounds.min.m)
	Point location(GETSIZEDRANDOM(x), GETSIZEDRANDOM(y));
#undef GETSIZEDRANDOM

	return Sentinel(location, sentinelSize);
}

// TODO: factor [food & mine] loops together (possibly make each a class?)
void GameWorld::FoodLoop()
{
	Timer foodTimer;

	while(!reset)
	{
		if(foodTimer.ResetIfHasElapsed(Config::Get().spawnPeriod))
		{
			Sentinel sentinel(get_new_sentinel(Config::Get().sentinelSize, Config::Get().worldBounds));
			while(Physics::AnyCollide(sentinel, gameObjects.physics))
			{
				sentinel = get_new_sentinel(Config::Get().sentinelSize, Config::Get().worldBounds);
				SDL_Delay(10);
			}

			DOLOCKEDZ(gameObjects,
				DOLOCKED(foodMutex,
					foods.push_back(Food(sentinel, Config::Get().spawnSize, get_food_type()));
					gameObjects.Add(*foods.rbegin());
				)
			)

			play_spawn_sound();
			Logger::Debug("Food spawn");
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
		if(mineTimer.ResetIfHasElapsed(Config::Get().spawnPeriod))
		{
			Sentinel sentinel(get_new_sentinel(Config::Get().sentinelSize, Config::Get().worldBounds));
			while(Physics::AnyCollide(sentinel, gameObjects.physics))
			{
				sentinel = get_new_sentinel(Config::Get().sentinelSize, Config::Get().worldBounds);
				SDL_Delay(10);
			}

			DOLOCKEDZ(gameObjects,
				mines.push_back(Mine(sentinel, Config::Get().spawnSize));
				gameObjects.Add(*mines.rbegin());
			)

			play_spawn_sound();
			Logger::Debug("Mine spawn");
		}

		SDL_Delay(100);
	}

	DOLOCKEDZ(gameObjects,
		gameObjects.RemoveRange(mines.begin(), mines.end());
	)
}

void GameWorld::Init(const bool survival)
{
	reset = false;

	if(survival)
		spawnThread = thread(boost::bind(&GameWorld::MineLoop, this));
	else
		spawnThread = thread(boost::bind(&GameWorld::FoodLoop, this));
}

GameWorld::GameWorld(ZippedUniqueObjectList& _gameObjects) :
	gameObjects(_gameObjects), player(GetCenter(), gameObjects)
{
	make_walls(walls);
	DOLOCKEDZ(gameObjects,
		gameObjects.AddRange(walls.begin(), walls.end());
	)
	
	Init(Config::Get().survival);
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

	Init(Config::Get().survival);
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
			Logger::Fatal("Invalid key somehow passed to get_direction_from_key");
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
	play_sound(Config::Get().resources.die);
}

static inline void play_eat_sound()
{
	play_sound(Config::Get().resources.eat);
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
				Food* const toRemove = reinterpret_cast<Food*>((o1.GetObjectType() == WorldObject::food) ? &o1 : &o2);
				for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
				{
					if(&*i == toRemove)
					{
						DOLOCKEDZ(gameObjects,
							gameObjects.Remove(*i);
						)
						foods.erase(i);
						break;
					}
				}
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
	return Point(Config::Get().worldBounds.max.x / 2, Config::Get().worldBounds.max.y / 2);
}
