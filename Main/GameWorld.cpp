#include "GameWorld.hpp"

#include "Event.hpp"
#include "Wall.hpp"

#include <boost/bind.hpp>
#include <boost/random.hpp>
#include <vector>

using namespace boost;
using namespace std;

namespace {
Logger::Handle logger = Logger::RequestHandle("GameWorld");
DEF_CONSTANT(unsigned int, foodAdditionPeriod, 8000)
DEF_CONSTANT(unsigned int, foodSize, 15)
DEF_CONSTANT(unsigned int, wallThickness, 10)
DEF_CONSTANT(Bounds, worldBounds, Bounds(Point(0, 0), Point(800, 600)))
// TODO: make this nicer
DEF_CONSTANT(Bounds, spawnBounds, Bounds(Point(wallThickness(), wallThickness()), \
	Point(800 - wallThickness(), 600 - wallThickness())))

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
	if(probability_hit(randnum, 1.0 / 10.0, randMax))
		return Food::celery();

	if(probability_hit(randnum, 1.0 / 5.0, randMax))
		return Food::donut();

	return Food::normal();
}
}

GameWorld::GameWorld(ObjectManager& objectManager) :
	player(objectManager)
{
	Reset(objectManager);
}

void GameWorld::Update(ObjectManager& objectManager)
{
	player.Update(objectManager);

	// TODO: map?
	for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
		delete_food_if_eaten(objectManager, foods, i);

	if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod()))
	{
		minstd_rand0 rand(time(NULL));

		Point foodLocation(
			rand() % ((spawnBounds().max.x - spawnBounds().min.x) - foodSize()) + spawnBounds().min.x,
			rand() % ((spawnBounds().max.y - spawnBounds().min.y) - foodSize()) + spawnBounds().min.y
		);

		Food::FoodInfo foodType = get_food_type(rand);

		Food* newFood = objectManager.Create(Food(foodLocation, foodSize(), foodType));
		foods.push_back(newFood);
	}
}

void GameWorld::Reset(ObjectManager& objectManager)
{
	player.Reset(objectManager);
	Event::RegisterPlayer(player);
	foods.clear();
	foodTimer.Reset();
	make_walls(objectManager);
}

bool GameWorld::Lost() const
{
	return player.IsDead();
}
