#include "GameWorld.hpp"

#include "Food.hpp"
#include "Logger.hpp"
#include "UniqueObjectList.hpp"
#include "Wall.hpp"

#include <boost/bind.hpp>
#include <boost/random.hpp>

using namespace boost;
using namespace std;

static Logger::Handle logger(Logger::RequestHandle("GameWorld"));

// GAMECONSTANT: food management
static const unsigned int foodAdditionPeriod(8000);
static const unsigned int foodSize(15);

static const unsigned int wallThickness(10);
static const Bounds worldBounds(Point(0, 0), Point(800, 600));

static inline void make_walls(GameWorld::WallBox& walls)
{
	walls[0] = Wall(Point(0, 0), wallThickness, worldBounds.max.y);
	walls[1] = Wall(Point(worldBounds.max.x - wallThickness, 0), wallThickness, worldBounds.max.y);
	walls[2] = Wall(Point(0, 0), worldBounds.max.x, wallThickness);
	walls[3] = Wall(Point(0, worldBounds.max.y - wallThickness), worldBounds.max.x, wallThickness);
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
	quit = false;
	foodTimer.Reset();
}

GameWorld::GameWorld(UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects) :
	player(GetCenter(), graphicsObjects, physicsObjects)
{
	make_walls(walls);
	graphicsObjects.addRange(walls.begin(), walls.end());
	physicsObjects.addRange(walls.begin(), walls.end());

	Init();
}

void GameWorld::Update(UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects)
{
	player.Update(graphicsObjects, physicsObjects);

	graphicsObjects.removeRange(foods.begin(), foods.end());
	physicsObjects.removeRange(foods.begin(), foods.end());
	for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
		if(i->IsEaten())
			foods.erase(i);
	graphicsObjects.addRange(foods.begin(), foods.end());
	physicsObjects.addRange(foods.begin(), foods.end());

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
			graphicsObjects.removeRange(foods.begin(), foods.end());
			physicsObjects.removeRange(foods.begin(), foods.end());
			foods.push_back(newFood);
			graphicsObjects.addRange(foods.begin(), foods.end());
			physicsObjects.addRange(foods.begin(), foods.end());

			physicsObjects.remove(*sentinel);
			sentinels.erase(sentinel);
		}
	}

	if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod))
	{
		sentinels.push_back(SentinelFood());
		send_sentinel(*sentinels.rbegin());
		physicsObjects.add(*sentinels.rbegin());
	}
}

void GameWorld::Reset(UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects)
{
	player.Reset(GetCenter(), graphicsObjects, physicsObjects);

	physicsObjects.removeRange(sentinels.begin(), sentinels.end());
	sentinels.clear();

	graphicsObjects.removeRange(foods.begin(), foods.end());
	physicsObjects.removeRange(foods.begin(), foods.end());
	foods.clear();

	Init();
}

void GameWorld::QuitNotify()
{
	quit = true;
}

static Direction get_direction_from_key(const SDLKey key)
{
	switch(key)
	{
		case SDLK_LEFT:
			return Direction::left;
			break;

		case SDLK_RIGHT:
			return Direction::right;
			break;

		case SDLK_UP:
			return Direction::up;
			break;

		case SDLK_DOWN:
			return Direction::down;
			break;

		default:
			logger.Fatal("Invalid key somehow passed to get_direction_from_key");
			return Direction::empty;
	}
}

void GameWorld::KeyNotify(SDLKey key, UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects)
{
	if(key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN)
	{
		player.ChangeDirection(get_direction_from_key(key), graphicsObjects, physicsObjects);
	}
}

bool GameWorld::Lost() const
{
	return player.IsDead();
}

bool GameWorld::QuitCalled() const
{
	return quit;
}

Point GameWorld::GetCenter() const
{
	return Point(worldBounds.max.x / 2, worldBounds.max.y / 2);
}
