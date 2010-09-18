#include "GameWorld.hpp"

#include "Wall.hpp"

#include <boost/random.hpp>
#include <vector>

using namespace boost;
using namespace std;

namespace {
DEF_CONSTANT(unsigned int, foodAdditionPeriod, 8000)
DEF_CONSTANT(unsigned int, foodSize, 15)
DEF_CONSTANT(unsigned int, wallThickness, 10)
DEF_CONSTANT(Bounds, worldBounds, Bounds(Point(0, 0), Point(800, 600)))

void make_walls(ObjectManager& objectManager)
{
	objectManager.Create(Wall(Point(0, 0), wallThickness(), worldBounds().max.y));
	objectManager.Create(Wall(Point(worldBounds().max.x - wallThickness(), 0), wallThickness(), worldBounds().max.y));
	objectManager.Create(Wall(Point(0, 0), worldBounds().max.x, wallThickness()));
	objectManager.Create(Wall(Point(0, worldBounds().max.y - wallThickness()), worldBounds().max.x, wallThickness()));
}

void delete_food_if_eaten(ObjectManager& objectManager, GameWorld::Menu& v, GameWorld::Menu::iterator food)
{
	if((*food)->IsEaten())
	{
		objectManager.Destroy(*food);
		v.erase(food);
	}
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
bool probability_hit(unsigned int& randnum, double probability, unsigned int randMax)
{
	unsigned int border = round(randMax * probability);
	if(randnum < border)
		return true;

	randnum -= border;
	return false;
}

Food::FoodInfo get_food_type(minstd_rand0& rand)
{
	const unsigned int randMax = 100;
	unsigned int randnum = rand() % (randMax + 1);

	if(probability_hit(randnum, 1.0 / 20.0, randMax))
		return Food::ice();

	if(probability_hit(randnum, 1.0 / 10.0, randMax))
		return Food::celery();

	if(probability_hit(randnum, 1.0 / 5.0, randMax))
		return Food::donut();

	return Food::normal();
}

void send_sentinel(SentinelFood*& sentinel, ObjectManager& objectManager)
{
	minstd_rand0 rand(time(NULL));

	Point foodLocation(
		rand() % ((worldBounds().max.x - worldBounds().min.x) - foodSize()) + worldBounds().min.x,
		rand() % ((worldBounds().max.y - worldBounds().min.y) - foodSize()) + worldBounds().min.y
	);

	sentinel = objectManager.Create(SentinelFood(foodLocation, foodSize()));
}
}

GameWorld::GameWorld() :
	logger(Logger::RequestHandle("GameWorld")), player(objectManager), eventHandler(*this)
{
	Reset();
}

void GameWorld::Update()
{
	objectManager.Update();
	eventHandler.Update();
	player.Update(objectManager);

	// TODO: map?
	for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
		delete_food_if_eaten(objectManager, foods, i);

	if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod()))
	{
		// TODO: change to have a list of sentinels,
		// so that even if one gets extremely bad luck,
		// it'll continue to try to appear
		/*if(sentinel)
		{
			if(!sentinel->IsInterfering())
			{
				minstd_rand0 rand(time(NULL));
				Food::FoodInfo foodType = get_food_type(rand);

				Food* newFood = objectManager.Create(Food(*sentinel, foodType));
				foods.push_back(newFood);

				objectManager.Destroy(sentinel);
				sentinel = NULL;
			}
			else
			{
				objectManager.Destroy(sentinel);
				send_sentinel(sentinel, objectManager);
			}
		}

		// if we've already sent a sentinel,
		// we might as well keep that one
		if(sentinel == NULL)
		{
			send_sentinel(sentinel, objectManager);
		}*/
	}
}

void GameWorld::Reset()
{
	//sentinel = NULL;
	quit = false;
	objectManager.Reset();
	player.Reset(objectManager);
	foods.clear();
	foodTimer.Reset();
	make_walls(objectManager);
}

void GameWorld::QuitNotify()
{
	quit = true;
}

void GameWorld::KeyNotify(SDLKey key)
{
	switch(key)
	{
		case SDLK_LEFT:
			player.ChangeDirection(Direction::left(), objectManager);
			break;

		case SDLK_RIGHT:
			player.ChangeDirection(Direction::right(), objectManager);
			break;

		case SDLK_UP:
			player.ChangeDirection(Direction::up(), objectManager);
			break;

		case SDLK_DOWN:
			player.ChangeDirection(Direction::down(), objectManager);
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
