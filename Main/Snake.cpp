#include "Screen.hpp"
#include "Snake.hpp"

#include <cassert>
#include <cstdlib>

#include <SDL/SDL.h>

using namespace std;

const Snake::Direction Snake::directions[] = {left, right, up, down};

Snake::Snake() :
	color(0, 255, 0)
{
	Reset();
}

void Snake::Reset()
{
	length = 1;
	path = vector<Direction>();
	// give it a random starting direction
	path.push_back(directions[rand() % 4]);
}
void Snake::GetInput()
{
	assert(!"Snake::GetInput");
}
void Snake::Update()
{
	assert(!"Snake::Update");
}
void Snake::Draw(Screen& target) const
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

	Block currentBlock(Point(location.x, location.y), target.blockWidth);
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
		SDL_FillRect(target.GetSurface(), currentBlock.GetRect(target), SDL_MapRGB(target.GetSurface()->format, color.red, color.green, color.blue));
	}
}
bool Snake::IsDead() const
{
	assert(!"Snake::Dead");
	return false;
}
