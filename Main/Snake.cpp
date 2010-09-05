#include <cassert>
#include <cstdlib>

#include <SDL/SDL.h>

#include "Common.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "Timer.hpp"

using namespace std;

const Snake::Direction Snake::directions[] = {left, right, up, down};

Snake::Snake() :
	color(0, 255, 0), screen(NULL)
{
	Reset();
}

void Snake::Reset()
{
	// reset to one segment
	length = 1;
	path = Path();
	path.push_back(SnakeSegment());
	head = path.begin();
	// give it a random starting direction
	direction = directions[rand() % countof(directions)];
}
void Snake::GetInput()
{
	assert(!"Snake::GetInput");
}
void Snake::Update()
{
	assert(direction == up || direction == down || direction == right || direction == left);

	// TODO: use different snake speeds
	if(moveTimer.ResetIfHasElapsed(125))
	{
		if(length > path.size())
		{
			path.push_back(SnakeSegment());
		}

		// rather than moving each tile individually, simply
		// make each one take the place of the one in front
		for(Path::iterator i = path.begin(); i != path.end() - 1; ++i)
		{
			(i + 1)->location = i->location;
		}
		switch(direction)
		{
			case left:
				--head->location.x;
				break;
			case right:
				++head->location.x;
				break;
			case up:
				--head->location.y;
				break;
			case down:
				++head->location.y;
				break;
		}
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

	head->location.x = screen->bottomRight.x / 2;
	head->location.y = screen->bottomRight.y / 2;
}
void Snake::Draw() const
{
	assert(screen != nullptr);

	for(Path::const_iterator i = path.begin(); i != path.end(); ++i)
	{
		screen->DrawRect(i->location, color);
	}
}
bool Snake::IsDead() const
{
	assert(screen != nullptr);
	// TODO: add self-collision detection

	// basic screen bounds check
	return (head->location.x < 0 || head->location.x > screen->bottomRight.x
		 || head->location.y < 0 || head->location.y > screen->bottomRight.y);
}
