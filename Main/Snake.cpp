#include <cassert>

#include <boost/random.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Physics.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "Timer.hpp"

using namespace std;

const Snake::Direction Snake::right(1, 0);
const Snake::Direction Snake::left(-1, 0);
const Snake::Direction Snake::up(0, -1);
const Snake::Direction Snake::down(0, 1);

Snake::Snake() :
	color(0, 255, 0), screen(NULL)
{
	Reset();
}

void Snake::AddSegment()
{
	// vectors move their elements when they are modified.
	// therefore, all elements must be removed and re-added

	for(Path::iterator segmentToRemove = path.begin(), end = path.end();
		segmentToRemove != end;
		++segmentToRemove)
	{
		PhysicsWorld::RemoveObject(*segmentToRemove);
	}

	path.push_back(SnakeSegment());

	for(Path::iterator segmentToAdd = path.begin(), end = path.end();
		segmentToAdd != end;
		++segmentToAdd)
	{
		PhysicsWorld::AddObject(*segmentToAdd);
	}
}
void Snake::Reset()
{
	dead = false;

	length = defaultLength;
	path = Path();
	// give it a head!
	AddSegment();

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

	// TODO: use different snake speeds
	if(moveTimer.ResetIfHasElapsed(125))
	{
		// TODO: discuss deque hack (only move path.begin() & tail)
		// if it's still feasible later

		if(length > path.size())
		{
			AddSegment();
		}

		// rather than moving each tile individually, simply
		// make each one take the place of the one in front
		for(Path::reverse_iterator i = path.rbegin() + 1, end = path.rend(); i != end; ++i)
		{
			if(i->IsDead())
				dead = true;

			Path::reverse_iterator last = i;
			--last;
			last->location = i->location;
		}
		if(path.rbegin()->IsDead())
			dead = true;

		apply_direction(*path.begin(), direction);
	}
	if(growTimer.ResetIfHasElapsed(4000))
	{
		++length;
		AddSegment();
	}
}
void Snake::SetRenderTarget(Screen& target)
{
	screen = &target;
}
void Snake::Center()
{
	// the snake can't be screen-centered w/o a screen
	assert(screen != nullptr);

	path.begin()->location.x = screen->bottomRight.x / 2;
	path.begin()->location.y = screen->bottomRight.y / 2;
}
void Snake::Draw() const
{
	assert(screen != nullptr);

	for(Path::const_iterator i = path.begin(); i != path.end(); ++i)
	{
		screen->Draw(*i, color);
	}
}
bool Snake::IsDead() const
{
	return dead;
}
