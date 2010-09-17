#include "GameWorld.hpp"

#include "Event.hpp"
#include "Wall.hpp"

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

void make_walls(World& world)
{
	world.Create(Wall(Point(0, 0), wallThickness(), worldBounds().max.y));
	world.Create(Wall(Point(worldBounds().max.x - wallThickness(), 0), wallThickness(), worldBounds().max.y));
	world.Create(Wall(Point(0, 0), worldBounds().max.x, wallThickness()));
	world.Create(Wall(Point(0, worldBounds().max.y - wallThickness()), worldBounds().max.x, wallThickness()));
}
}

GameWorld::GameWorld(World& world) :
	player(world)
{
	Reset(world);
}

void GameWorld::Update(World& world)
{
	player.Update(world);

	// TODO: reduce spawn area (store walls in GameWorld)
	Bounds spawnBounds = worldBounds();
	// TODO: map?
	for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
	{
		if((*i)->IsEaten())
		{
			world.Destroy(*i);
			foods.erase(i);
		}
	}

	if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod()))
	{
		minstd_rand0 rand(time(NULL));

		Point foodLocation(
				rand() % (worldBounds().max.x - foodSize()),
				rand() % (worldBounds().max.y - foodSize())
			);

		Food* newFood = world.Create(Food(foodLocation, foodSize()));
		foods.push_back(newFood);
	}
}

void GameWorld::Reset(World& world)
{
	player.Reset(world);
	Event::RegisterPlayer(player);
	foods.clear();
	foodTimer.Reset();
	make_walls(world);
}

bool GameWorld::Lost() const
{
	return player.IsDead();
}
