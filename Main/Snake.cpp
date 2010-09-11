#include <cassert>

#include <boost/random.hpp>
#include <SDL/SDL.h>

#include "Snake.hpp"
#include "Color24.hpp"
#include "Common.hpp"
#include "Logger.hpp"
#include "Point.hpp"
#include "Timer.hpp"

using namespace std;

Snake::Snake(Point loc)
	: logger(Logger::RequestHandle("Snake"))
{
	Reset(loc);
}

void Snake::AddTailSegment(Point location, Direction direction)
{
	{
	SnakeSegment newSegment(location, direction);
	path.push_front(newSegment);
	}
	Head().AddToWorld();
}
void Snake::Grow()
{
	const unsigned int growthAmount = 30;
	length += growthAmount;
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
	for(Path::iterator i = path.begin(), end = path.end(); i != end; ++i)
		if(i->HasEaten())
			Grow();

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
	for(Path::const_iterator i = path.begin(), end = path.end(); i != end; ++i)
	{
		if(i->IsDead())
			return true;
	}
	return false;
}
