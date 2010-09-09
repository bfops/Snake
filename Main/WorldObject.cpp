#include <stdexcept>
#include <string>

#include "Common.hpp"
#include "WorldObject.hpp"

using namespace std;

WorldObject::~WorldObject()
{
}

void WorldObject::Draw(Screen& target) const
{
	SDL_Surface* surface = target.GetSurface();

	SDL_Rect rect;
	rect.w = width;
	rect.h = height;
	rect.x = location.x;
	rect.y = location.y;

	if(SDL_FillRect(surface, &rect, color.GetRGBMap(surface)) == -1)
	{
		string error = "Error drawing to screen: ";
		error += SDL_GetError();
		throw runtime_error(error.c_str());
	}
}
