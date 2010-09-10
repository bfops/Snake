#include <stdexcept>
#include <string>

#include "Common.hpp"
#include "Logger.hpp"
#include "Physics.hpp"
#include "WorldObject.hpp"

using namespace std;

WorldObject::WorldObject(ObjectType _type) :
	inPhysics(false), type(_type)
{
}
WorldObject::WorldObject(const WorldObject& obj) :
	inPhysics(obj.inPhysics), location(obj.location), width(obj.width), height(obj.height), color(obj.color)
{
	if(inPhysics)
		PhysicsWorld::Add(*this);
}
WorldObject::~WorldObject()
{
	if(inPhysics)
		PhysicsWorld::Remove(*this);
}

void WorldObject::AddToWorld()
{
	PhysicsWorld::Add(*this);
	inPhysics = true;
}
void WorldObject::RemoveFromWorld()
{
	PhysicsWorld::Remove(*this);
	inPhysics = false;
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
