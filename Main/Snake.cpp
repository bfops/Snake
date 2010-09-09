#include <cassert>

#include <boost/random.hpp>
#include <SDL/SDL.h>

#include "Color24.hpp"
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

Color24 headColor(160, 160, 160);
Color24 bodyColor(0, 255, 0);

Snake::Snake(Point loc)
{
	Reset(loc);
}

void Snake::AddTailSegment(Point location)
{
	// vectors move their elements when they are modified.
	// therefore, all elements must be removed and re-added
	// to the Physics World, because otherwise its pointers
	// will be invalid.

	if(path.size() > 0)
	{
		PhysicsWorld::Remove(*path.begin());
		if(path.size() > 1)
			PhysicsWorld::RemoveGroup(*(path.begin() + 1));
	}

	SnakeSegment newSegment;
	newSegment.location = location;
	path.push_back(newSegment);

	PhysicsWorld::PhysicsGroup bodyGroup;
	for(Path::iterator segmentToAdd = path.begin() + 1, end = path.end(); segmentToAdd != end; ++segmentToAdd)
	{
		bodyGroup.push_back(&*segmentToAdd);
	}
	PhysicsWorld::Add(bodyGroup);
	PhysicsWorld::Add(*path.begin());

	const unsigned int headLength = 15;
	if(path.size() <= headLength)
	{
		path.rbegin()->color = headColor;
	}
	else
	{
		path.rbegin()->color = bodyColor;
	}
}
void Snake::Grow()
{
	const unsigned int growthAmount = 30;
	length += growthAmount;
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
		// TODO: different things will make it grow differently
		const unsigned int growthAmount = 15;
		length += growthAmount;
	}

	while(moveTimer.ResetIfHasElapsed(1000 / speed))
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
			i->width = (i + 1)->width;
			i->height = (i + 1)->height;
			i->location = (i + 1)->location;
		}
		apply_direction(path.begin()->location, direction);
	}
}
void Snake::Draw(Screen& target) const
{
	for(Path::const_iterator i = path.begin(), end = path.end(); i != end; ++i)
		i->Draw(target);
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
