#include "WorldObject.hpp"

#include "Logger.hpp"
#include "Screen.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_video.h>

#ifdef MSVC
#pragma warning(pop)
#endif

using namespace std;

WorldObject::WorldObject(ObjectType _type)
{
	type = _type;
}

WorldObject::WorldObject(ObjectType _type, const Color24 _color)
{
	type = _type;
	color = _color;
}

WorldObject::~WorldObject()
{
}

WorldObject::ObjectType WorldObject::GetObjectType() const
{
	return type;
}

void WorldObject::CollisionHandler(WorldObject&) const
{
	Logger::Fatal(boost::format("Subclass type %1% did not override CollisionHandler()") % GetObjectType());
}

void WorldObject::CollisionHandler(const Food&)
{
}

void WorldObject::CollisionHandler(const Mine&)
{
}

void WorldObject::CollisionHandler(const Sentinel&)
{
}

void WorldObject::CollisionHandler(const SnakeSegment&)
{
}

void WorldObject::CollisionHandler(const Wall&)
{
}

void WorldObject::Draw(const Screen& target) const
{
	// TODO: lock when doing this!
	SDL_Surface* const surface = target.GetSurface();

	SDL_Rect rect;
	rect.x = bounds.min.x;
	rect.y = bounds.min.y;
	rect.w = bounds.max.x - bounds.min.x;
	rect.h = bounds.max.y - bounds.min.y;

	if(SDL_FillRect(surface, &rect, color.GetRGBMap(surface)) == -1)
		Logger::Fatal(boost::format("Error drawing to screen: %1%") % SDL_GetError());

	SDL_FreeSurface(surface);
}

Bounds WorldObject::GetBounds() const
{
	return bounds;
}
