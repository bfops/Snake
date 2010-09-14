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
using namespace boost;

namespace {
Logger::Handle logger = Logger::RequestHandle("Snake");
DEF_CONSTANT(unsigned int, defaultLength, 90)
DEF_CONSTANT(unsigned int, snakeWidth, 20)
DEF_CONSTANT(unsigned int, speedupPeriod, 16000)
DEF_CONSTANT(unsigned int, speedupAmount, 23)
DEF_CONSTANT(unsigned int, growthCap, 110)
DEF_CONSTANT(double, linearGrowthRate, 10.0 / 29.0)
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

Direction get_random_direction()
{
	const static Direction directions[] = {Direction::left(), Direction::right(), Direction::up(), Direction::down()};
	uint32_t randomNumber = minstd_rand(time(NULL))();
	return directions[randomNumber % countof(directions)];
}
void Snake::Reset(Point headLocation)
{
	moveTimer.Reset();
	growTimer.Reset();
	speedupTimer.Reset();

	speed = 100;

	length = 0;
	projectedLength = defaultLength();
	path.clear();

	AddTailSegment(headLocation, get_random_direction());
}
void Snake::ChangeDirection(Direction newDirection)
{
	// TODO: change so that the new segment takes on the
	// "intersection" block between the new and old segment

	// TODO: if players enter two directions quickly enough,
	// they can u-turn into themselves. Fix.

	Direction direction(Head().GetDirection());

	if(newDirection != direction && newDirection != -direction)
	{
		// the point to start is the _direction_
		// side of the current head
		Bounds head = Head().GetHeadSquare();
		Side startSide = head.GetSide(newDirection);

		SnakeSegment newSegment(startSide.min, newDirection, snakeWidth());
		newSegment.AddToWorld();
		path.push_front(newSegment);
	}
}

void Snake::Update()
{
	for(Path::iterator i = path.begin(), end = path.end(); i != end; ++i)
	{
		if(i->HasEaten())
		{
			// TODO: this should be fetched from the food!
			double foodConstant = 1;
			double growthConstant;
			if(projectedLength < growthCap() / linearGrowthRate())
				// 10 / 29 is magically derived from some testing
				growthConstant = projectedLength * linearGrowthRate();
			else
				growthConstant = growthCap();
			Grow(round(foodConstant * growthConstant));

			i->Digest();
		}
	}

	while(speedupTimer.ResetIfHasElapsed(speedupPeriod()))
		speed += speedupAmount();

	while(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		// when there is a request for more length,
		// simply don't shrink the tail
		if(length < projectedLength)
			++length;
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
