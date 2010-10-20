#include "Snake.hpp"

#include "Common.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Side.hpp"
#include "ZippedUniqueObjectList.hpp"

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace boost;

static Logger::Handle logger(Logger::RequestHandle("Snake"));

// GAMECONSTANT: snake management constants
static const unsigned int defaultLength(90);
static const unsigned int snakeWidth(20);
static const unsigned int speedupPeriod(14000);
static const unsigned int speedupAmount(18);
static const unsigned int growthCap(100);
static const double linearGrowthRate(10.0 / 29.0);

Snake::Snake(Point center, ZippedUniqueObjectList& gameObjects)
{
	Init(center, gameObjects);
}

void Snake::AddSegment(Point location, Direction direction, ZippedUniqueObjectList& gameObjects)
{
	SnakeSegment newSegment(this, location, direction, snakeWidth);

	gameObjects.removeRange(path.begin(), path.end());

	path.push_front(newSegment);

	gameObjects.addRange(path.begin(), path.end());
}

void Snake::Grow(int amount)
{
	if((int)projectedLength + amount < (int)defaultLength)
		projectedLength = defaultLength;
	else
		projectedLength += amount;
}

inline SnakeSegment& Snake::Head()
{
	return path.front();
}

inline SnakeSegment& Snake::Tail()
{
	return path.back();
}

static inline Direction get_random_direction()
{
	const static Direction directions[] = {Direction::left, Direction::right, Direction::up, Direction::down};
	uint32_t randomNumber = minstd_rand(time(NULL))();
	return directions[randomNumber % countof(directions)];
}

void Snake::Init(Point center, ZippedUniqueObjectList& gameObjects)
{
	points = 0;
	Point headLocation = center;

	moveTimer.Reset();
	speedupTimer.Reset();

	speed = 100;

	length = 0;
	projectedLength = defaultLength;

	AddSegment(headLocation, get_random_direction(), gameObjects);
}

void Snake::Reset(Point center, ZippedUniqueObjectList& gameObjects)
{
	gameObjects.removeRange(path.begin(), path.end());
	path.clear();

	Init(center, gameObjects);
}

void Snake::ChangeDirection(Direction newDirection, ZippedUniqueObjectList& gameObjects)
{
	Direction direction(Head().GetDirection());

	if(newDirection != direction && newDirection != -direction && Head().GetWidth() >= 2 * snakeWidth)
	{
		// the point to start is the _direction_
		// side of the current head
		Bounds headBlock = Head().GetHeadSquare();
		// remove the head block from this segment
		Head().SetHeadSide(headBlock.GetSide(-direction));
		// take on the head block from the old segment
		Side startSide = headBlock.GetSide(-newDirection);

		AddSegment(startSide.min, newDirection, gameObjects);
		// stretch this segment so that its initial size
		// is enough to cover the head block
		Head().SetHeadSide(headBlock.GetSide(newDirection));
	}
}

static inline unsigned int get_bounded_index(const int unboundedIndex, const int arraySize)
{
	if(unboundedIndex < 0)
		return get_bounded_index(unboundedIndex + arraySize, arraySize);

	return unboundedIndex % arraySize;
}

static Direction get_turned_direction(Direction direction, Direction turn)
{
	assert(turn == Direction::left || turn == Direction::right);

	const Direction directions[] = {Direction::left, Direction::up, Direction::right, Direction::down};
	const int nextDirection = (turn == Direction::left ? -1 : 1);

	for(unsigned int i = 0; i < countof(directions); ++i)
		if(direction == directions[i])
			return directions[get_bounded_index(i + nextDirection, countof(directions))];

	return Direction::empty;
}

void Snake::Turn(Direction turn, ZippedUniqueObjectList& gameObjects)
{
	Direction direction(Head().GetDirection());
	ChangeDirection(get_turned_direction(direction, turn), gameObjects);
}

void Snake::Update(ZippedUniqueObjectList& gameObjects, unsigned int ms)
{
	speedupTimer.Update(ms);
	moveTimer.Update(ms);

	while(speedupTimer.ResetIfHasElapsed(speedupPeriod))
		speed += speedupAmount;

	while(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		++Head();

		if(length > projectedLength)
		{
			--Tail();
			--length;
		}

		// if we need more length, just don't shrink the tail
		if(length < projectedLength)
		{
			++length;
		}
		else
		{
			--Tail();

			if(Tail().IsEmpty())
			{
				gameObjects.remove(Tail());
				path.pop_back();
			}
		}
	}
}

void Snake::EatFood(const Food& foodObj)
{
	const double foodGrowthConstant = foodObj.GetCalories();
	const double baseUncappedGrowth = projectedLength * linearGrowthRate;
	const double baseRealGrowth = min((double)growthCap, baseUncappedGrowth);
	const int growthAmount = round(baseRealGrowth * foodGrowthConstant);
	const int pointsGained = foodObj.GetPoints();

	points += pointsGained;
	Grow(growthAmount);

	DEBUGLOG(logger, format("Grew by %1%") % growthAmount);
	DEBUGLOG(logger, format("Total points: %1%") % points);
}
