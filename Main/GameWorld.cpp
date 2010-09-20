#include "GameWorld.hpp"

#include "collision.h"
#include "custom_algorithm.hpp"
#include "Wall.hpp"

#include <boost/random.hpp>
#include <cassert>
#include <vector>

using namespace boost;
using namespace std;

// TODO: factor out all the common delete/add
// code for vectors into a common function

namespace {
DEF_CONSTANT(Point, screenBounds, Point(800, 600))

namespace Graphics {
void Update(UniqueObjectList& gameObjects, Screen& screen)
{
	screen.Clear();

	// TODO: re-bind
	for(UniqueObjectList::iterator i = gameObjects.begin(), end = gameObjects.end(); i != end; ++i)
		(*i)->Draw(screen);

	screen.Update();
}
}

namespace Physics {
namespace {
inline CollidableObject world_to_collidable_object(const WorldObject* w)
{
	CollidableObject ret;

	Bounds bounds = w->GetBounds();
	ret.min.x = bounds.min.x;
	ret.min.y = bounds.min.y;
	ret.max.x = bounds.max.x;
	ret.max.y = bounds.max.y;

	return ret;
}

void handle_potential_collision(WorldObject* o1, WorldObject* o2)
{
	CollidableObject c1 = world_to_collidable_object(o1);
	CollidableObject c2 = world_to_collidable_object(o2);

	if(does_collide(&c1, &c2))
	{
		o1->CollisionHandler(*o2);
		o2->CollisionHandler(*o1);
	}
}

inline void collide_with_subsequent_objects(UniqueObjectList::iterator collider, UniqueObjectList::iterator end)
{
	// TODO: re-bind
	for(UniqueObjectList::iterator collidee = collider + 1; collidee != end; ++collidee)
		handle_potential_collision(*collider, *collidee);
}
}

void Update(UniqueObjectList& gameObjects)
{
	// don't try the last gameObjects, since all have been checked against it
	for(UniqueObjectList::iterator collider = gameObjects.begin(), end = gameObjects.end() - 1; collider != end; ++collider)
		collide_with_subsequent_objects(collider, gameObjects.end());
}
}

DEF_CONSTANT(unsigned int, foodAdditionPeriod, 8000)
DEF_CONSTANT(unsigned int, foodSize, 15)
DEF_CONSTANT(unsigned int, wallThickness, 10)
DEF_CONSTANT(Bounds, worldBounds, Bounds(Point(0, 0), Point(800, 600)))

void make_walls(GameWorld::WallBox& walls)
{
	walls[0] = Wall(Point(0, 0), wallThickness(), worldBounds().max.y);
	walls[1] = Wall(Point(worldBounds().max.x - wallThickness(), 0), wallThickness(), worldBounds().max.y);
	walls[2] = Wall(Point(0, 0), worldBounds().max.x, wallThickness());
	walls[3] = Wall(Point(0, worldBounds().max.y - wallThickness()), worldBounds().max.x, wallThickness());
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

void send_sentinel(SentinelFood& sentinel)
{
	minstd_rand0 rand(time(NULL));

	Point foodLocation(
		rand() % ((worldBounds().max.x - worldBounds().min.x) - foodSize()) + worldBounds().min.x,
		rand() % ((worldBounds().max.y - worldBounds().min.y) - foodSize()) + worldBounds().min.y
	);

	sentinel = SentinelFood(foodLocation, foodSize());
}
}

GameWorld::GameWorld(UniqueObjectList& gameObjects) :
	logger(Logger::RequestHandle("GameWorld")),	screen(screenBounds().x, screenBounds().y),
	player(GetCenter(), gameObjects), eventHandler(*this)
{
	make_walls(walls);
	for_each(walls.begin(), walls.end(), bind(&UniqueObjectList::add, &gameObjects, _1));
	sentinelSent = false;
	Reset(gameObjects);
}

void GameWorld::Update(UniqueObjectList& gameObjects)
{
	eventHandler.Update(gameObjects);

	// TODO: fix pausing
	if(!paused)
	{
		Physics::Update(gameObjects);
		Graphics::Update(gameObjects, screen);
		player.Update(gameObjects);

		for_each(foods.begin(), foods.end(), bind(&UniqueObjectList::remove, &gameObjects, _1));
		for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
			if(i->IsEaten())
				foods.erase(i);
		for_each(foods.begin(), foods.end(), bind(&UniqueObjectList::add, &gameObjects, _1));

		// TODO: change to have a list of sentinels,
		// so we can have many trying to appear,
		// if one takes REALLY long
		if(sentinelSent)
		{
			if(sentinel.IsInterfering())
			{
				gameObjects.remove(sentinel);
				send_sentinel(sentinel);
				gameObjects.add(sentinel);
			}
			else
			{
				minstd_rand0 rand(time(NULL));
				Food::FoodInfo foodType = get_food_type(rand);

				Food newFood(sentinel, foodType);
				for_each(foods.begin(), foods.end(), bind(&UniqueObjectList::remove, &gameObjects, _1));
				foods.push_back(newFood);
				for_each(foods.begin(), foods.end(), bind(&UniqueObjectList::add, &gameObjects, _1));

				gameObjects.remove(sentinel);
				sentinelSent = false;
			}
		}

		if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod()))
		{
			// if we've already sent a sentinel,
			// we might as well keep that one
			if(!sentinelSent)
			{
				send_sentinel(sentinel);
				gameObjects.add(sentinel);
				sentinelSent = true;
			}
		}
	}
}

void GameWorld::Reset(UniqueObjectList& gameObjects)
{
	paused = false;

	if(sentinelSent)
		gameObjects.remove(sentinel);
	sentinelSent = false;

	quit = false;
	player.Reset(GetCenter(), gameObjects);

	for_each(foods.begin(), foods.end(), bind(&UniqueObjectList::remove, &gameObjects, _1));
	foods.clear();
	foodTimer.Reset();
}

void GameWorld::QuitNotify()
{
	quit = true;
}

void GameWorld::KeyNotify(SDLKey key, UniqueObjectList& gameObjects)
{
	if(key == SDLK_p)
		paused = !paused;

	if(!paused)
	{
		switch(key)
		{
			case SDLK_LEFT:
				player.ChangeDirection(Direction::left(), gameObjects);
				break;

			case SDLK_RIGHT:
				player.ChangeDirection(Direction::right(), gameObjects);
				break;

			case SDLK_UP:
				player.ChangeDirection(Direction::up(), gameObjects);
				break;

			case SDLK_DOWN:
				player.ChangeDirection(Direction::down(), gameObjects);
				break;

			default:
				break;
		}
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
	return Point(screenBounds().x / 2, screenBounds().y / 2);
}
