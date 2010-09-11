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

const Snake::Direction Snake::right(1, 0);
const Snake::Direction Snake::left(-1, 0);
const Snake::Direction Snake::up(0, -1);
const Snake::Direction Snake::down(0, 1);

namespace {
Color24 headColor(160, 160, 160);
Color24 bodyColor(0, 255, 0);
}

Snake::Snake(Point loc)
	: logger(Logger::RequestHandle("Snake"))
{
	Reset(loc);
}

void Snake::AddTailSegment(Point location)
{
	SnakeSegment newSegment;
	newSegment.location = location;
	path.push_back(newSegment);
	path.rbegin()->AddToWorld();

	const unsigned int headLength = 15;

	path.rbegin()->color = (path.size() <= headLength) ? headColor : bodyColor;
}
void Snake::Grow(size_t amount)
{
	const unsigned int factor = 30;
	length += factor * amount;
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
	size_t pathsThatHaveEaten = std::count_if(path.begin(), path.end(), boost::bind(&SnakeSegment::HasEaten, _1));
	Grow(pathsThatHaveEaten);

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
			logger.Debug("Adding new segment\n");
			AddTailSegment(Point());
			logger.Debug("New segment added\n");
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

bool Snake::IsDead() const
{
	return any(path.begin(), path.end(), boost::bind(&SnakeSegment::IsDead, _1));
}
