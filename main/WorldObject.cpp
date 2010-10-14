#include "WorldObject.hpp"

#include "Logger.hpp"
#include "Screen.hpp"

#include <SDL_video.h>

using namespace std;

static Logger::Handle logger(Logger::RequestHandle("WorldObject"));

WorldObject::WorldObject(ObjectType _type) :
	type(_type)
{
}

WorldObject::ObjectType WorldObject::GetObjectType() const
{
	return type;
}

void WorldObject::Draw(Screen& target) const
{
	SDL_Surface* surface = target.GetSurface();

	SDL_Rect rect;
	rect.w = bounds.max.x - bounds.min.x;
	rect.h = bounds.max.y - bounds.min.y;
	rect.x = bounds.min.x;
	rect.y = bounds.min.y;

	if(SDL_FillRect(surface, &rect, color.GetRGBMap(surface)) == -1)
	{
		string error = "Error drawing to screen: ";
		error += SDL_GetError();
		logger.Fatal(error.c_str());
	}
}

Bounds WorldObject::GetBounds() const
{
	return bounds;
}

Color24 WorldObject::GetColor() const
{
	return color;
}
