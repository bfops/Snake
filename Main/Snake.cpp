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
	path.push_back(Point());
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
		// only the head and tail need to be moved!
		// The body might as well be stationary

		Point headPoint = *head;
		switch(direction)
		{
			case left:
				--headPoint.x;
				break;
			case right:
				++headPoint.x;
				break;
			case up:
				--headPoint.y;
				break;
			case down:
				++headPoint.y;
				break;
		}
		path.push_front(headPoint);
		path.pop_back();
		head = path.begin();
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

	head->x = screen->bottomRight.x / 2;
	head->y = screen->bottomRight.y / 2;
}
void Snake::Draw() const
{
	class Block
	{
		private:
			mutable SDL_Rect rect;

		public:
			Point index;

			Block(Point _index, unsigned short width) :
			rect()
			{
				index.x = _index.x;
				index.y = _index.y;
				rect.h = width;
				rect.w = width;
			}

			const SDL_Rect* GetRect(const Screen& target) const
			{
				Point screenIndex = target.ResolveIndex(index);
				rect.x = screenIndex.x;
				rect.y = screenIndex.y;

				return &rect;
			}
			SDL_Rect* GetRect(const Screen& target)
			{
				return const_cast<SDL_Rect*>(const_cast<const Block*>(this)->GetRect(target));
			}
	};

	assert(screen != nullptr);

	Block currentBlock(Point(), screen->blockWidth);
	for(Path::const_iterator i = path.begin(); i != path.end(); ++i)
	{
		currentBlock.index = *i;
		// TODO: check for errors here
		SDL_FillRect(screen->GetSurface(), currentBlock.GetRect(*screen), SDL_MapRGB(screen->GetSurface()->format, color.red, color.green, color.blue));
	}
}
bool Snake::IsDead() const
{
	assert(screen != nullptr);
	// TODO: add self-collision detection

	// basic screen bounds check
	return (head->x < 0 || head->x > screen->bottomRight.x
		 || head->y < 0 || head->y > screen->bottomRight.y);
}
