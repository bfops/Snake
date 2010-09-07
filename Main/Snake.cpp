#include <cassert>

#include <boost/random.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "DebugLogger.hpp"
#include "Physics.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "Timer.hpp"

using namespace std;

const Snake::Direction Snake::right(1, 0);
const Snake::Direction Snake::left(-1, 0);
const Snake::Direction Snake::up(0, -1);
const Snake::Direction Snake::down(0, 1);

Snake::Snake(Point loc) :
	color(0, 255, 0)
{
	Reset(loc);
}

void Snake::AddTailSegment(Point location)
{
	// vectors move their elements when they are modified.
	// therefore, all elements must be removed and re-added
	// to the Physics World, because otherwise its pointers
	// will be invalid.

	for(Path::iterator segmentToRemove = path.begin(), end = path.end();
		segmentToRemove != end;
		++segmentToRemove)
	{
		PhysicsWorld::RemoveObject(*segmentToRemove);
	}

	SnakeSegment newSegment;
	newSegment.location = location;
	path.push_back(newSegment);

	for(Path::iterator segmentToAdd = path.begin(), end = path.end();
		segmentToAdd != end;
		++segmentToAdd)
	{
		PhysicsWorld::AddObject(*segmentToAdd);
	}
}
void Snake::Reset(Point headLocation)
{
	#ifdef NDEBUG
	const unsigned int defaultLength = 3;
	#else
	const unsigned int defaultLength = 15;
	#endif

	length = defaultLength;
	path = Path();
	// give it a head!
	AddTailSegment(headLocation);

	// give it a random starting direction
	const static Direction directions[] = {left, right, up, down};
	boost::uint32_t randomNumber = boost::minstd_rand(boost::int32_t(time(NULL)))();
	direction = directions[randomNumber % countof(directions)];
}
void Snake::ChangeDirection(Snake::Direction newDirection)
{
	if(newDirection != -direction)
		direction = newDirection;
}

static void apply_direction(SnakeSegment& segment, const Vector2D& direction)
{
	segment.location.x += direction.x;
	segment.location.y += direction.y;
}
void Snake::Update()
{
	assert(direction == up || direction == down || direction == right || direction == left);

	const unsigned int growthPeriod = 4000;
	if(growTimer.ResetIfHasElapsed(growthPeriod))
	{
		++length;
	}
	// in Hertz
	const unsigned int movementFrequency = 8;
	// TODO: use different snake speeds
	if(moveTimer.ResetIfHasElapsed(1000 / movementFrequency))
	{
		// TODO: discuss deque hack (only move path.begin() & tail)
		// if it's still feasible later

		if(length > path.size())
		{
			DebugLogger::Log("Adding new segment (%u)\n", path.size() + 1);
			{
				DebugLogger::Indent indent;
				AddTailSegment(Point());
			}
			DebugLogger::Log("New segment added\n");
		}

		// move each tile to the location of the next tile in line
		// the magic +1 is since the tail segment has nothing to which
		// to pass its location
		for(Path::reverse_iterator currentSegment = path.rbegin() + 1, head = path.rend();
			currentSegment != head;
			++currentSegment)
		{
			Path::reverse_iterator lastSegment = currentSegment - 1;
			lastSegment->location = currentSegment->location;
		}

		apply_direction(*path.begin(), direction);
	}
}
void Snake::Draw(Screen& target) const
{
	for(Path::const_iterator i = path.begin(); i != path.end(); ++i)
	{
		target.Draw(*i, color);
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
