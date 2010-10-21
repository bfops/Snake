#include "GameWorld.hpp"

#include "Common.hpp"
#include "custom_algorithm.hpp"
#include "EventHandler.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Wall.hpp"
#include "ZippedUniqueObjectList.hpp"

#include <functional>
#include <boost/random.hpp>
#include <SDL_timer.h>
#include <SDL_mixer.h>

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("GameWorld"));

// GAMECONSTANT: food management
#ifdef SURVIVAL
static const unsigned int foodAdditionPeriod(6000);
#else
static const unsigned int foodAdditionPeriod(8000);
#endif
static const unsigned int foodSize(15);

static const unsigned int wallThickness(10);
static const Bounds worldBounds(Point(0, 0), Point(800, 600));

static inline void make_walls(GameWorld::WallBox& walls)
{
	walls[0] = Wall(Point(0, 0), wallThickness, worldBounds.max.y);
	walls[1] = Wall(Point(worldBounds.max.x - wallThickness, 0), wallThickness, worldBounds.max.y);
	walls[2] = Wall(Point(0, 0), worldBounds.max.x, wallThickness);
	walls[3] = Wall(Point(0, worldBounds.max.y - wallThickness), worldBounds.max.x, wallThickness);
#ifdef COOLERWALLS
	walls[4] = Wall(Point(worldBounds.max.x / 2, 0), wallThickness, worldBounds.max.y / 4);
	walls[5] = Wall(Point(worldBounds.max.x / 2, worldBounds.max.y * 3 / 4), wallThickness, worldBounds.max.y / 4);
	walls[6] = Wall(Point(0, worldBounds.max.y / 2), worldBounds.max.x / 4, wallThickness);
	walls[7] = Wall(Point(worldBounds.max.x * 3 / 4, worldBounds.max.y / 2), worldBounds.max.x / 4, wallThickness);
#endif
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

static Food::FoodInfo get_food_type(minstd_rand0& rand)
{
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

static void send_sentinel(SentinelFood& sentinel)
{
	minstd_rand0 rand(time(NULL));

	Point foodLocation(
		rand() % ((worldBounds.max.x - worldBounds.min.x) - foodSize) + worldBounds.min.x,
		rand() % ((worldBounds.max.y - worldBounds.min.y) - foodSize) + worldBounds.min.y
	);

	sentinel = SentinelFood(foodLocation, foodSize);
}

void GameWorld::Init()
{
	foodTimer.Reset();
}

GameWorld::GameWorld(ZippedUniqueObjectList& gameObjects) :
	player(GetCenter(), gameObjects)
{
	make_walls(walls);
	gameObjects.addRange(walls.begin(), walls.end());

	Init();
}

// _filename is a const char*
static int sound_playing_thread(void* _filename)
{
	const char* const filename = reinterpret_cast<const char* const>(_filename);
	Mix_Chunk* const sound = Mix_LoadWAV(filename);
	int channel;
	if(sound == NULL || (channel = Mix_PlayChannel(-1, sound, 0)) == -1)
	{
		logger.Fatal(format("Error playing \"%1%\": %2%") % filename % Mix_GetError());
		return 1;
	}

	while(Mix_Playing(channel))
		SDL_Delay(50);

	Mix_FreeChunk(sound);

	return 0;
}

static inline void play_sound(const char* filename)
{
	SDL_CreateThread(sound_playing_thread, reinterpret_cast<void*>(const_cast<char*>(filename)));
}

static inline void play_food_sound()
{
	play_sound("resources/food appear.wav");
}

void GameWorld::Update(ZippedUniqueObjectList& gameObjects, unsigned int ms)
{
	foodTimer.Update(ms);
	player.Update(gameObjects, ms);

	// TODO: use interrupts, rather than this check-loop
	gameObjects.removeRange(foods.begin(), foods.end());
	for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
		if(i->IsEaten())
			foods.erase(i);
	gameObjects.addRange(foods.begin(), foods.end());

	for(SentinelList::iterator sentinel = sentinels.begin(), end = sentinels.end(); sentinel != end; ++sentinel)
	{
		if(sentinel->IsInterfering())
		{
			send_sentinel(*sentinel);
		}
		else
		{
			minstd_rand0 rand(time(NULL));
			Food::FoodInfo foodType = get_food_type(rand);

			Food newFood(*sentinel, foodType);
			gameObjects.removeRange(foods.begin(), foods.end());
			foods.push_back(newFood);
			play_food_sound();
			gameObjects.addRange(foods.begin(), foods.end());

			gameObjects.physics.remove(*sentinel);
			sentinels.erase(sentinel);
		}
	}

	if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod))
	{
		sentinels.push_back(SentinelFood());
		send_sentinel(*sentinels.rbegin());
		gameObjects.physics.add(*sentinels.rbegin());
	}
}

void GameWorld::Reset(ZippedUniqueObjectList& gameObjects)
{
	player.Reset(GetCenter(), gameObjects);

	gameObjects.physics.removeRange(sentinels.begin(), sentinels.end());
	sentinels.clear();

	gameObjects.removeRange(foods.begin(), foods.end());
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

void GameWorld::CollisionNotify(const WorldObject::ObjectType o1, const WorldObject::ObjectType o2)
{
	const unsigned int collisionType = o1 | o2;
	const bool selfCollide = !(collisionType & ~o1);

	if(collisionType & WorldObject::snake)
	{
		if(selfCollide || collisionType & WorldObject::wall	|| collisionType & WorldObject::mine)
		{
			EventHandler::GetCurrentEventHandler()->LossNotify();
			play_death_sound();
		}
		else if(collisionType & WorldObject::food)
		{
			play_eat_sound();
		}
	}
}

void GameWorld::KeyNotify(const SDLKey key, ZippedUniqueObjectList& gameObjects)
{
	if(key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN)
		player.ChangeDirection(get_direction_from_key(key), gameObjects);
}

void GameWorld::MouseNotify(Uint8 button, ZippedUniqueObjectList& gameObjects)
{
	if(button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT)
		player.Turn(get_direction_from_button(button), gameObjects);
}

Point GameWorld::GetCenter() const
{
	return Point(worldBounds.max.x / 2, worldBounds.max.y / 2);
}
