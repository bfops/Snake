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

namespace {
Logger::Handle logger = Logger::RequestHandle("Snake");
#ifdef DEBUG
DEF_CONSTANT(unsigned int, defaultLength, 224)
#else
DEF_CONSTANT(unsigned int, defaultLength, 44)
#endif
DEF_CONSTANT(unsigned int, snakeWidth, 15)
DEF_CONSTANT(unsigned int, speedupPeriod, 10000)
DEF_CONSTANT(unsigned int, growthPeriod, 4000)
DEF_CONSTANT(unsigned int, growthAmount, 15)
}

Snake::Snake(Point loc)
{
	Reset(loc);
}

void Snake::AddTailSegment(Point location, Direction direction)
{
	path.push_front(SnakeSegment(location, direction, snakeWidth()));
	Head().AddToWorld();
}
void Snake::Grow(size_t amount)
{
	length += amount;
}

inline SnakeSegment& Snake::Head()
{
	return path.front();
}
inline SnakeSegment& Snake::Tail()
{
	return path.back();
}

void Snake::Reset(Point headLocation)
{
	moveTimer.Reset();
	growTimer.Reset();
	speedupTimer.Reset();

	speed = 100;

	length = defaultLength();
	path.clear();

	const static Direction directions[] = {Direction::left, Direction::right, Direction::up, Direction::down};
	boost::uint32_t randomNumber = boost::minstd_rand(boost::int32_t(time(NULL)))();
	Direction randomStartDirection(directions[randomNumber % countof(directions)]);

	AddTailSegment(headLocation, randomStartDirection);
}
void Snake::ChangeDirection(Direction newDirection)
{
	// TODO: change so that the new segment takes on the
	// "intersection" block between the new and old segment

	// TODO: if players enter two directions quickly enough,
	// they can u-turn into themselves. Fix.

	Direction direction(Head().GetDirection());
	Bounds bounds(Head().GetBounds());

	if(newDirection != direction && newDirection != -direction)
	{
		Point newSegmentStart;
		if(direction == Direction::left || direction == Direction::right)
		{
			if(newDirection == Direction::up)
				newSegmentStart.y = bounds.min.y;
			else if(newDirection == Direction::down)
				newSegmentStart.y = bounds.max.y;

			if(direction == Direction::left)
				newSegmentStart.x = bounds.min.x;
			else
				newSegmentStart.x = bounds.max.x - snakeWidth();
		}
		else if(direction == Direction::up || direction == Direction::down)
		{
			if(newDirection == Direction::left)
				newSegmentStart.x = bounds.min.x;
			else if(newDirection == Direction::right)
				newSegmentStart.x = bounds.max.x;

			if(direction == Direction::up)
				newSegmentStart.y = bounds.min.y;
			else
				newSegmentStart.y = bounds.max.y - snakeWidth();
		}

		path.push_front(SnakeSegment(newSegmentStart, newDirection, snakeWidth()));
		Head().AddToWorld();
	}
}

void Snake::Update()
{
	for(Path::iterator i = path.begin(), end = path.end(); i != end; ++i)
	{
		// TODO: different foods do different amounts
		if(i->HasEaten())
		{
			i->Digest();
			Grow(30);
		}
	}

	while(speedupTimer.ResetIfHasElapsed(speedupPeriod()))
	{
		speed += 20;
	}

	while(growTimer.ResetIfHasElapsed(growthPeriod()))
	{
		Grow(growthAmount());
	}

	while(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		// when there is a request for more length,
		// simply don't shrink the tail
		if(length > 0)
			--length;
		else
		{
			--Tail();
			if(Tail().IsEmpty())
			{
				Tail().RemoveFromWorld();
				path.pop_back();
			}
		}

		++Head();
	}
}

bool Snake::IsDead() const
{
	return any(path.begin(), path.end(), boost::bind(&SnakeSegment::IsDead, _1));
}
