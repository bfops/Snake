#include <cassert>
#include <cstddef>

#include <boost/bind/bind.hpp>
#include <boost/random.hpp>
#include <SDL/SDL.h>

#include "Snake.hpp"
#include "Color24.hpp"
#include "Common.hpp"
#include "Logger.hpp"
#include "Point.hpp"
#include "Timer.hpp"

#include "custom_algorithm.hpp"

using namespace std;

Snake::Snake(Point loc)
	: logger(Logger::RequestHandle("Snake"))
{
	Reset(loc);
}

void Snake::AddTailSegment(Point location, Direction direction)
{
	SnakeSegment newSegment(location, direction);
	path.push(newSegment);
	newSegment.AddToWorld();
}
void Snake::Grow(size_t amount)
{
	length += amount;
}

inline SnakeSegment& Snake::Head()
{
	return *path.begin();
}
inline SnakeSegment& Snake::Tail()
{
	return *path.rbegin();
}

void Snake::Reset(Point headLocation)
{
	moveTimer.Reset();
	growTimer.Reset();
	speedupTimer.Reset();

	speed = 100;

#ifdef NDEBUG
	const unsigned int defaultLength = 44;
#else
	const unsigned int defaultLength = 224;
#endif

	length = defaultLength;
	path.clear();

	const static Direction directions[] = {Direction::left, Direction::right, Direction::up, Direction::down};
	boost::uint32_t randomNumber = boost::minstd_rand(boost::int32_t(time(NULL)))();
	Direction randomStartDirection(directions[randomNumber % countof(directions)]);

	AddTailSegment(headLocation, randomStartDirection);
}
void Snake::ChangeDirection(Direction newDirection)
{
	// TODO: if players enter two directions quickly enough,
	// they can u-turn into themselves. Fix.

	assert(!"Snake::ChangeDirection");
}

void Snake::Update()
{
	size_t segmentsWhichHaveEaten = std::count_if(path.begin(), path.end(), boost::bind(&SnakeSegment::HasEaten, _1));
	Grow(segmentsWhichHaveEaten * 30);

	const unsigned int speedupPeriod = 10000;
	while(speedupTimer.ResetIfHasElapsed(speedupPeriod))
	{
		speed += 20;
	}

	const unsigned int growthPeriod = 4000;
	while(growTimer.ResetIfHasElapsed(growthPeriod))
	{
		const unsigned int growthAmount = 15;
		length += growthAmount;
	}

	while(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		// when there is a request for more length,
		// simply don't shrink the tail
		if(length > 0)
			--length;
		else
			--Tail();

		++Head();
	}
}

bool Snake::IsDead() const
{
	return any(path.begin(), path.end(), boost::bind(&SnakeSegment::IsDead, _1));
}
