#include "Snake.hpp"

#include "Common.hpp"
#include "Config.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Side.hpp"
#include "ZippedUniqueObjectList.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

using namespace std;
using namespace boost;

const static Direction directions[] = {Direction::left, Direction::right, Direction::up, Direction::down};

Snake::Snake(const Point center, ZippedUniqueObjectList& gameObjects)
{
	Init(center, gameObjects);
}

void Snake::AddSegment(const Point location, const Direction direction, ZippedUniqueObjectList& gameObjects)
{
	const SnakeSegment newSegment(this, location, direction, Config::Get().snake.width);
	
	DOLOCKEDZ(gameObjects,
		gameObjects.RemoveRange(path.begin(), path.end());
		path.push_front(newSegment);
		gameObjects.AddRange(path.begin(), path.end());
	)
}

void Snake::Grow(const int amount)
{
	if((int)projectedLength + amount < (int)Config::Get().snake.startingLength)
		projectedLength = Config::Get().snake.startingLength;
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
	uint32_t randomNumber = minstd_rand(time(NULL))();
	return directions[randomNumber % countof(directions)];
}

void Snake::Init(const Point center, ZippedUniqueObjectList& gameObjects)
{
	points = 0;
	Point headLocation = center;

	moveTimer.Reset();
	speedupTimer.Reset();
	pointTimer.Reset();

	speed = Config::Get().snake.startingSpeed;

	length = 0;
	projectedLength = Config::Get().snake.startingLength;
	
	AddSegment(headLocation, get_random_direction(), gameObjects);
}

void Snake::Reset(const Point center, ZippedUniqueObjectList& gameObjects)
{
	moveTimer.Reset();
	speedupTimer.Reset();
	pointTimer.Reset();
	
	DOLOCKEDZ(gameObjects,
		gameObjects.RemoveRange(path.begin(), path.end());
	)
	path.clear();

	Init(center, gameObjects);
}

void Snake::EmptyTailNotify(ZippedUniqueObjectList& gameObjects)
{
	DOLOCKEDZ(gameObjects,
		gameObjects.RemoveRange(path.begin(), path.end());
	)
	path.pop_back();
	DOLOCKEDZ(gameObjects,
		gameObjects.AddRange(path.begin(), path.end());
	)
}

void Snake::ChangeDirection(const Direction newDirection, ZippedUniqueObjectList& gameObjects)
{
	const Direction direction(Head().GetDirection());

	if(newDirection != direction && newDirection != -direction && Head().GetWidth() >= 2 * Config::Get().snake.width)
	{
		// the point to start is the _direction_
		// side of the current head
		const Bounds headBlock = Head().GetHeadSquare();
		// remove the head block from this segment
		Head().SetHeadSide(headBlock.GetSide(-direction));
		// take on the head block from the old segment
		const Side startSide = headBlock.GetSide(-newDirection);

		AddSegment(startSide.min, newDirection, gameObjects);
		// stretch this segment so that its initial size
		// is enough to cover the head block
		Head().SetHeadSide(headBlock.GetSide(newDirection));
	}
}

static inline unsigned int get_bounded_index(const int unboundedIndex, const unsigned int arraySize)
{
	if(unboundedIndex < 0)
		return get_bounded_index(unboundedIndex + arraySize, arraySize);

	return unsigned int(unboundedIndex) % arraySize;
}

static Direction get_turned_direction(const Direction direction, const Direction turn)
{
	assert(turn == Direction::left || turn == Direction::right);

	const Direction directions[] = {Direction::left, Direction::up, Direction::right, Direction::down};
	const int nextDirection = (turn == Direction::left ? -1 : 1);

	for(unsigned int i = 0; i < countof(directions); ++i)
		if(direction == directions[i])
			return directions[get_bounded_index(i + nextDirection, countof(directions))];

	return Direction::empty;
}

void Snake::Turn(const Direction turn, ZippedUniqueObjectList& gameObjects)
{
	const Direction direction(Head().GetDirection());
	ChangeDirection(get_turned_direction(direction, turn), gameObjects);
}

void Snake::Update(ZippedUniqueObjectList& gameObjects)
{
	if(pointTimer.ResetIfHasElapsed(Config::Get().pointGainPeriod))
	{
		points += Config::Get().pointGainAmount;
		Logger::Debug(format("%1% points gained! (total %2%)") % Config::Get().pointGainAmount % points);
	}

	if(speedupTimer.ResetIfHasElapsed(Config::Get().snake.speedupPeriod))
	{
		speed += Config::Get().snake.speedupAmount;
		Logger::Debug(format("Speeding up by %1%") % Config::Get().snake.speedupAmount);
	}

	if(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		Head().Grow(gameObjects);

		if(length > projectedLength)
		{
			Tail().Shrink(gameObjects);
			--length;
		}

		// if we need more length, just don't shrink the tail
		if(length < projectedLength)
			++length;
		else
			Tail().Shrink(gameObjects);
	}
}

void Snake::EatFood(const Food& foodObj)
{
	const double foodGrowthConstant = foodObj.GetCalories();
	const double baseUncappedGrowth = projectedLength * Config::Get().snake.growthRate;
	const double baseRealGrowth = min((double)Config::Get().snake.growthCap, baseUncappedGrowth);
	const int growthAmount = round(baseRealGrowth * foodGrowthConstant);
	const int pointsGained = foodObj.GetPoints();

	// if adding points would make you go below 0
	if(pointsGained < 0 && (unsigned int)(-pointsGained) > points)
		points = 0;
	else
		points += pointsGained;

	Grow(growthAmount);

	Logger::Debug(format("Growing by %1%") % growthAmount);
	Logger::Debug(format("Got %1% points! (total %2%)") % pointsGained % points);
}
