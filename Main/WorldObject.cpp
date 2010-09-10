#include <stdexcept>
#include <string>

#include "WorldObject.hpp"

#include "Common.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include "Physics.hpp"

using namespace std;

WorldObject::WorldObject(ObjectType _type) :
	inPhysics(false), inGraphics(false), type(_type)
{
}
WorldObject::WorldObject(const WorldObject& obj) :
	inPhysics(obj.inPhysics), inGraphics(obj.inGraphics),
	location(obj.location), width(obj.width), height(obj.height), color(obj.color)
{
	if(inPhysics)
		PhysicsWorld::Add(*this);
	if(inGraphics)
		GraphicsWorld::Add(*this);
}
WorldObject::~WorldObject()
{
	if(inPhysics)
		PhysicsWorld::Remove(*this);
	if(inGraphics)
		GraphicsWorld::Remove(*this);
}

void WorldObject::AddToWorld()
{
	// TODO: get mad if they're doin it wrong

	if(!inPhysics)
		PhysicsWorld::Add(*this);

	if(!inGraphics)
		GraphicsWorld::Add(*this);

	inPhysics = inGraphics = true;
}
void WorldObject::RemoveFromWorld()
{
	// TODO: get mad if they're doin it wrong

	if(inPhysics)
		PhysicsWorld::Remove(*this);

	if(inGraphics)
		GraphicsWorld::Remove(*this);

	inPhysics = inGraphics = false;
}

WorldObject::ObjectType WorldObject::GetObjectType() const
{
	return type;
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
