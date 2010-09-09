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
	headColor(160, 160, 160), bodyColor(0, 255, 0)
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
	const unsigned int defaultLength = 44;
	#else
	const unsigned int defaultLength = 224;
	#endif

	length = defaultLength;
	path = Path();
	AddTailSegment(headLocation);

	// give it a random starting direction
	const static Direction directions[] = {left, right, up, down};
	boost::uint32_t randomNumber = boost::minstd_rand(boost::int32_t(time(NULL)))();
	direction = directions[randomNumber % countof(directions)];
	if(direction != left && direction != right)
		swap(path.begin()->width, path.begin()->height);
}
void Snake::ChangeDirection(Direction newDirection)
{
	// TODO: if players enter two directions quickly enough,
	// they can u-turn into themselves. Fix.

	if(newDirection != direction && newDirection != -direction)
	{
		unsigned int width = path.begin()->width;
		unsigned int height = path.begin()->height;

		// TODO: OH GOD MAKE LESS UGLY.
		// This can be fixed with some trippy algebra;
		// I believe it involves reflection about a diagonal
		// line which intersects the corner the new and
		// old direction share. However, if  you can
		// clean it up, please do.

		Path::iterator currentSegment = path.begin();
		Path::iterator end = path.end();
		if(newDirection == left || newDirection == right)
		{
			for(unsigned int i = 0; i < width && currentSegment != end; ++i, ++currentSegment)
			{
				if(newDirection == left)
					currentSegment->location.x += i;
				else
					currentSegment->location.x += (width - 1 - i);

				if(direction == up)
					currentSegment->location.y -= i;
				else
					currentSegment->location.y -= (width - 1 - i);

				swap(currentSegment->width, currentSegment->height);
			}
		}
		else if(newDirection == up || newDirection == down)
		{
			for(unsigned int i = 0; i < height && currentSegment != end; ++i, ++currentSegment)
			{
				if(direction == left)
					currentSegment->location.x -= i;
				else
					currentSegment->location.x -= (height - 1 - i);

				if(newDirection == up)
					currentSegment->location.y += i;
				else
					currentSegment->location.y += (height - 1 - i);

				swap(currentSegment->width, currentSegment->height);
			}
		}
		direction = newDirection;
	}
}

static void apply_direction(Point& location, const Vector2D& direction)
{
	location.x += direction.x;
	location.y += direction.y;
}
void Snake::Update()
{
	const unsigned int growthPeriod = 4000;
	while(growTimer.ResetIfHasElapsed(growthPeriod))
	{
		// TODO: different things will make it grow differently
		const unsigned int growthAmount = 15;
		length += growthAmount;
	}
	// in Hertz
	const unsigned int movementFrequency = 128;
	// TODO: use different snake speeds
	while(moveTimer.ResetIfHasElapsed(1000 / movementFrequency))
	{
		if(length > path.size())
		{
			DebugLogger::Log("Adding new segment\n");
			{
				DebugLogger::Indent indent;
				AddTailSegment(Point());
			}
			DebugLogger::Log("New segment added\n");
		}

		// each segment assumes the properties of the one in front of it
		for(Path::reverse_iterator i = path.rbegin(), end = path.rend() - 1; i != end; ++i)
		{
			*i = *(i + 1);
		}
		apply_direction(path.begin()->location, direction);
	}
}
void Snake::Draw(Screen& target) const
{
	const unsigned int headSize = max(path.begin()->width, path.begin()->height);

	// color the first n segments the color of the head
	unsigned int n = 0;
	for(Path::const_iterator i = path.begin(), end = path.end(); i != end; ++i, ++n)
	{
		if(n < headSize)
			target.Draw(*i, headColor);
		else
			target.Draw(*i, bodyColor);
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
