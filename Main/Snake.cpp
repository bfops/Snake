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
	color(0, 255, 0), screen(NULL), timer()
{
	Reset();
}

void Snake::Reset()
{
	length = 1;
	path = vector<Direction>();
	// give it a random starting direction
	path.push_back(directions[rand() % countof(directions)]);
}
void Snake::GetInput()
{
	assert(!"Snake::GetInput");
}
void Snake::Update()
{
	assert(*path.begin() == up || *path.begin() == down || *path.begin() == right || *path.begin() == left);

	// TODO: use different snake speeds
	if(timer.ResetIfHasElapsed(125))
	{
		switch(*path.begin())
		{
			case left:
				--location.x;
				break;
			case right:
				++location.x;
				break;
			case up:
				--location.y;
				break;
			case down:
				++location.y;
				break;
		}
	}
}
void Snake::SetRenderTarget(Screen& target)
{
	screen = &target;
}
void Snake::Center()
{
	// the snake can only be centered if you know how big the screen is
	assert(screen != nullptr);

	location.x = screen->bottomRight.x / 2;
	location.y = screen->bottomRight.y / 2;
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

	Block currentBlock(Point(location.x, location.y), screen->blockWidth);
	for(Path::const_iterator i = path.begin(); i != path.end(); ++i)
	{
		if(i != path.begin())
		{
			assert(*i == up || *i == down || *i == right || *i == left);

			if(*i == up)
				--currentBlock.index.y;
			else if(*i == down)
				++currentBlock.index.y;
			else if(*i == left)
				--currentBlock.index.x;
			else
				++currentBlock.index.x;
		}
		// TODO: check for errors here
		SDL_FillRect(screen->GetSurface(), currentBlock.GetRect(*screen), SDL_MapRGB(screen->GetSurface()->format, color.red, color.green, color.blue));
	}
}
bool Snake::IsDead() const
{
	assert(screen != nullptr);
	// TODO: add self-collision detection

	// basic screen bounds check
	return (location.x < 0 || location.y < 0 || location.x > screen->bottomRight.x || location.y > screen->bottomRight.y);
}
