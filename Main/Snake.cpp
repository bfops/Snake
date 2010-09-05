#include <cassert>

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

void Snake::Reset()
{
	dead = false;
	// remove snake from physics world
	for(Path::iterator currentSegment = path.begin(); currentSegment != path.end(); ++currentSegment)
	{
		PhysicsWorld::RemoveObject(*currentSegment);
	}

	length = defaultLength;
	path = Path();
	path.push_back(SnakeSegment());

	PhysicsWorld::AddObject(*path.begin());

	// give it a random starting direction
	const static Direction directions[] = {left, right, up, down};
	direction = directions[rand() % countof(directions)];
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
			path.push_back(SnakeSegment());
			// add the new segment to the physical world
			PhysicsWorld::AddObject(*path.rbegin());
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
		path.push_back(*path.rbegin());
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
