#include "GameWorld.hpp"

#include "Food.hpp"
#include "Logger.hpp"
#include "Wall.hpp"
#include "ZippedUniqueObjectList.hpp"

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
	foodTimer.Reset();
	lost = false;
}

GameWorld::GameWorld(ZippedUniqueObjectList& gameObjects) :
	player(*this, GetCenter(), gameObjects)
{
	make_walls(walls);
	gameObjects.addRange(walls.begin(), walls.end());

	Init();
}

void GameWorld::Update(ZippedUniqueObjectList& gameObjects, unsigned int ms)
{
	foodTimer.Update(ms);
	player.Update(gameObjects, ms);

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

Direction get_direction_from_button(Uint8 button)
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

void GameWorld::KeyNotify(SDLKey key, ZippedUniqueObjectList& gameObjects)
{
	if(key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN)
		player.ChangeDirection(get_direction_from_key(key), gameObjects);
}

void GameWorld::MouseNotify(Uint8 button, ZippedUniqueObjectList& gameObjects)
{
	if(button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT)
		player.Turn(get_direction_from_button(button), gameObjects);
}

void GameWorld::LossNotify()
{
	lost = true;
}

bool GameWorld::Lost() const
{
	return lost;
}

Point GameWorld::GetCenter() const
{
	return Point(worldBounds.max.x / 2, worldBounds.max.y / 2);
}
