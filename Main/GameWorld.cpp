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

template <typename Iter>
static inline void add_range_to_object_list(Iter start, Iter end, UniqueObjectList& gameObjects)
{
	for_each(start, end, bind(&UniqueObjectList::add, &gameObjects, _1));
}

template <typename Iter>
static inline void remove_range_from_object_list(Iter start, Iter end, UniqueObjectList& gameObjects)
{
	for_each(start, end, bind(&UniqueObjectList::remove, &gameObjects, _1));
}

template <typename _T>
static inline void add_vector_to_object_list(vector<_T>& v, UniqueObjectList& gameObjects)
{
	add_range_to_object_list(v.begin(), v.end(), gameObjects);
}

template <typename _T>
static inline void remove_vector_from_object_list(vector<_T>& v, UniqueObjectList& gameObjects)
{
	remove_range_from_object_list(v.begin(), v.end(), gameObjects);
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

GameWorld::GameWorld(UniqueObjectList& gameObjects) :
	player(GetCenter(), gameObjects)
{
	make_walls(walls);
	add_range_to_object_list(walls.begin(), walls.end(), gameObjects);

	Init();
}

void GameWorld::Update(UniqueObjectList& gameObjects)
{
	player.Update(gameObjects);

	remove_vector_from_object_list(foods, gameObjects);
	for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
		if(i->IsEaten())
			foods.erase(i);
	add_vector_to_object_list(foods, gameObjects);

	for(SentinelList::iterator sentinel = sentinels.begin(), end = sentinels.end(); sentinel != end; ++sentinel)
	{
		if(sentinel->IsInterfering())
		{
			gameObjects.remove(*sentinel);
			send_sentinel(*sentinel);
			gameObjects.add(*sentinel);
		}
		else
		{
			minstd_rand0 rand(time(NULL));
			Food::FoodInfo foodType = get_food_type(rand);

			Food newFood(*sentinel, foodType);
			remove_vector_from_object_list(foods, gameObjects);
			foods.push_back(newFood);
			add_vector_to_object_list(foods, gameObjects);

			gameObjects.remove(*sentinel);
			sentinels.erase(sentinel);
		}
	}

	if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod))
	{
		sentinels.push_back(SentinelFood());
		send_sentinel(*sentinels.rbegin());
		gameObjects.add(*sentinels.rbegin());;
	}
}

void GameWorld::Reset(UniqueObjectList& gameObjects)
{
	remove_vector_from_object_list(sentinels, gameObjects);
	sentinels.clear();

	player.Reset(GetCenter(), gameObjects);

	remove_vector_from_object_list(foods, gameObjects);
	foods.clear();

	Init();
}

void GameWorld::QuitNotify()
{
	quit = true;
}

void GameWorld::KeyNotify(SDLKey key, UniqueObjectList& gameObjects)
{
	switch(key)
	{
		case SDLK_LEFT:
			player.ChangeDirection(Direction::left, gameObjects);
			break;

		case SDLK_RIGHT:
			player.ChangeDirection(Direction::right, gameObjects);
			break;

		case SDLK_UP:
			player.ChangeDirection(Direction::up, gameObjects);
			break;

		case SDLK_DOWN:
			player.ChangeDirection(Direction::down, gameObjects);
			break;

		default:
			break;
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
