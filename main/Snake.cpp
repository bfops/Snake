#include "Snake.hpp"

#include "Common.hpp"
#include "Config.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Line.hpp"
#include "ZippedUniqueObjectList.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/bind.hpp>
#include <boost/random.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

using boost::minstd_rand;

const static Direction directions[] = {Direction::left, Direction::right, Direction::up, Direction::down};

Snake::Snake(ZippedUniqueObjectList& gameObjects)
{
	Init(gameObjects);
}

void Snake::AddSegment(ZippedUniqueObjectList& gameObjects)
{
	DOLOCKED(pathMutex,
		const Direction& direction = Head().direction;
		// we want to start at the back end of the head
		const SnakeSegment newSegment(this, Head().GetTailSide().min, direction, 0,
			Config::Get().snake.width, Config::Get().snake.color);
	
		path.insert(++path.begin(), newSegment);
		DOLOCKEDZ(gameObjects,
			gameObjects.Add(Growable());
		)
	)
}

void Snake::AddHead(const Point location, const Direction direction, ZippedUniqueObjectList& gameObjects)
{
	unsigned short width = Config::Get().snake.width;
	const SnakeSegment newSegment(this, location, direction, width, width, Config::Get().snake.head.color);
	
	DOLOCKED(pathMutex,
		path.push_front(newSegment);
		DOLOCKEDZ(gameObjects,
			gameObjects.Add(Head());
		)
	)
}

SnakeSegment& Snake::Head()
{
	return path.front();
}

SnakeSegment& Snake::Tail()
{
	return path.back();
}

SnakeSegment& Snake::Growable()
{
	return *++path.begin();
}

SnakeSegment& Snake::Shrinkable()
{
	return path.back();
}

static inline Direction get_random_direction()
{
	const uint32_t randomNumber = minstd_rand(time(NULL))();
	return directions[randomNumber % countof(directions)];
}

static inline Point get_head_location()
{
	const unsigned long width = Config::Get().snake.width;
	// middle of the screen
	Point startingPoint(Config::Get().screen.w / 2, Config::Get().screen.h / 2);
	// account for size
	startingPoint.x -= width / 2;
	startingPoint.y -= width / 2;

	return startingPoint;
}

void Snake::Init(ZippedUniqueObjectList& gameObjects)
{
	points = 0;

	moveTimer.Reset();
	speedupTimer.Reset();
	pointTimer.Reset();

	speed = Config::Get().snake.startingSpeed;

	length = 0;
	targetLength = Config::Get().snake.startingLength;
	
	const Direction direction = get_random_direction();
	DOLOCKED(pathMutex,
		AddHead(get_head_location(), direction, gameObjects);
		AddSegment(gameObjects);
	)
}

void Snake::Reset(ZippedUniqueObjectList& gameObjects)
{
	DOLOCKED(pathMutex,
		DOLOCKEDZ(gameObjects,
			gameObjects.RemoveRange(path.begin(), path.end());
		)
		path.clear();

		Init(gameObjects);
	)
}

void Snake::RemoveTail(ZippedUniqueObjectList& gameObjects)
{
	DOLOCKED(pathMutex,
		DOLOCKEDZ(gameObjects,
			gameObjects.Remove(Tail());
		)
		path.pop_back();
	)
}

void Snake::ChangeDirection(const Direction newDirection, ZippedUniqueObjectList& gameObjects)
{
	DOLOCKED(pathMutex,
		Direction& oldDirection = Head().direction;
		// the new direction and old direction can't be both horizontal nor both vertical
		// the new segment must be long enough to not collide with another segment if it turns
		if(newDirection.IsHorizontal() ^ oldDirection.IsHorizontal() &&
			Growable().GetLength() >= Config::Get().snake.width)
		{
			oldDirection = newDirection;
			AddSegment(gameObjects);
		}
	)
}

// essentially "modulo" with negative numbers
static inline unsigned long get_bounded_index(const long unboundedIndex, const unsigned long arraySize)
{
	if(unboundedIndex < 0)
		return get_bounded_index(unboundedIndex + arraySize, arraySize);

	return static_cast<unsigned long>(unboundedIndex) % arraySize;
}

static Direction get_turned_direction(const Direction direction, const Direction turn)
{
	// must turn either left or right
	assert(turn.IsHorizontal());

	const Direction directions[] = {Direction::left, Direction::up, Direction::right, Direction::down};
	// if we're turning left, we're going backwards through the array, else forwards
	const short nextDirection = (turn == Direction::left ? -1 : 1);

	for(unsigned short i = 0; i < countof(directions); ++i)
		if(direction == directions[i])
			return directions[get_bounded_index(i + nextDirection, countof(directions))];

	return Direction::empty;
}

void Snake::Turn(const Direction turn, ZippedUniqueObjectList& gameObjects)
{
	DOLOCKED(pathMutex,
		const Direction direction = Head().direction;
	)

	ChangeDirection(get_turned_direction(direction, turn), gameObjects);
}

void Snake::Update(ZippedUniqueObjectList& gameObjects)
{
	if(pointTimer.ResetIfHasElapsed(Config::Get().pointGainPeriod))
	{
		DOLOCKED(attribMutex,
			points += Config::Get().pointGainAmount;
			Logger::Debug(boost::format("%1% points gained! (total %2%)")
				% Config::Get().pointGainAmount % points);
		)
	}

	if(speedupTimer.ResetIfHasElapsed(Config::Get().snake.speedupPeriod))
	{
		DOLOCKED(attribMutex,
			speed += Config::Get().snake.speedupAmount;
			Logger::Debug(boost::format("Speeding up by %1%") % Config::Get().snake.speedupAmount);
		)
	}

	if(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		DOLOCKED(pathMutex,
			Head().Move();
			Growable().Grow();

			DOLOCKED(attribMutex,
				if(length > targetLength)
				{
					if(Shrinkable().Shrink())
						RemoveTail(gameObjects);
					--length;
				}

				// if we need more length, just don't shrink the tail
				if(length < targetLength)
					++length;
				else
					if(Shrinkable().Shrink())
						RemoveTail(gameObjects);
			)
		)
	}
}

// add _change_ to _original_. If doing so goes below _min_, set it to _min_ instead
template <typename _T, typename _X>
void SumUp(const _T change, _X& original, const _X min)
{
	// if the change is so negative that it exceeds our gain
	if(change < 0 && -change > original - min)
		original = min;
	else
		original += change;
}

void Snake::EatFood(const Food& foodObj)
{
	const Config::SnakeData& snakeData = Config::Get().snake;

	const double baseUncappedGrowth = targetLength * snakeData.growthRate;
	const double baseRealGrowth = std::min((double)snakeData.growthCap, baseUncappedGrowth);
	const long growthAmount = intRound(baseRealGrowth * foodObj.GetLengthFactor());
	const long long pointChange = foodObj.GetPointChange();
	const short speedChange = foodObj.GetSpeedChange();
	const unsigned long long defaultPoints = 0;

	DOLOCKED(attribMutex,
		SumUp(pointChange, points, defaultPoints);
		SumUp(growthAmount, targetLength, snakeData.startingLength);
		SumUp(speedChange, speed, snakeData.startingSpeed);
	)

	Logger::Debug(boost::format("Growing by %1%") % growthAmount);
	Logger::Debug(boost::format("Got %1% points! (total %2%)") % pointChange % points);
	Logger::Debug(boost::format("Speeding up by %1%") % speedChange);
}
