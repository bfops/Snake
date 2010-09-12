#include "World.hpp"
#include "WorldObject.hpp"
#include "collision.h"

#include <vector>
#include <boost/bind/bind.hpp>

#include "custom_algorithm.hpp"
#include "Logger.hpp"

using namespace std;
using namespace boost;

namespace World {
namespace {
typedef vector<WorldObject*> ObjectList;
ObjectList objects;
Logger::Handle logger = Logger::RequestHandle("World");

static inline bool object_exists(const WorldObject* obj)
{
	return in(objects.begin(), objects.end(), obj);
}

void Add(WorldObject& obj)
{
	assert(!object_exists(&obj));

	objects.push_back(&obj);
	logger.Debug(boost::format("Type %1% object %2% added") % obj.GetObjectType() % &obj);
}

void Remove(WorldObject& obj)
{
	if(unordered_find_and_remove(objects, &obj))
		logger.Debug(boost::format("Type %1% object %2% removed") % obj.GetObjectType() % &obj);
	else
		logger.Debug("Invalid object specified");
}

namespace GraphicsWorld {
// TODO: just move the damn screen in?
void Update(Screen& target)
{
	target.Clear();

	std::for_each(objects.begin(), objects.end(),
		boost::bind(&WorldObject::Draw, _1, boost::ref(target))
	);

	target.Update();
}
}

namespace PhysicsWorld {
static inline CollidableObject world_to_collidable_object(const WorldObject* w)
{
	CollidableObject ret;

	ret.Min.x = w->GetMinBounds().x;
	ret.Min.y = w->GetMinBounds().y;
	ret.Max.x = w->GetMaxBounds().x;
	ret.Max.y = w->GetMaxBounds().y;

	return ret;
}

static inline void handle_potential_collision(WorldObject* o1, WorldObject* o2)
{
	CollidableObject c1 = world_to_collidable_object(o1);
	CollidableObject c2 = world_to_collidable_object(o2);

	if(does_collide(&c1, &c2))
	{
		o1->CollisionHandler(*o2);
		o2->CollisionHandler(*o1);
	}
}

static inline void collide_with_subsequent_objects(ObjectList::iterator collider)
{
	std::for_each(collider + 1, objects.end(), boost::bind(handle_potential_collision, *collider, _1));
}

void Update()
{
	for(ObjectList::iterator collider = objects.begin(), lastGroup = objects.end() - 1; collider != lastGroup; ++collider)
		collide_with_subsequent_objects(collider);
}

}
}

void Update(Screen& target)
{
	PhysicsWorld::Update();
	GraphicsWorld::Update(target);
}

WorldObject::WorldObject(ObjectType _type) :
	inWorld(false), type(_type)
{
}

WorldObject::WorldObject(const WorldObject& obj) :
	inWorld(obj.inWorld), minBounds(obj.minBounds), maxBounds(obj.maxBounds), color(obj.color)
{
	if(inWorld)
		World::Add(*this);
}

WorldObject& WorldObject::operator=(const WorldObject& o)
{
	inWorld = o.inWorld;
	type = o.type;
	minBounds = o.minBounds;
	maxBounds = o.maxBounds;
	color = o.color;
	return *this;
}

WorldObject::~WorldObject()
{
	if(inWorld)
		World::Remove(*this);
}

void WorldObject::AddToWorld()
{
	if(!inWorld)
		World::Add(*this);

	inWorld = true;
}
void WorldObject::RemoveFromWorld()
{
	if(inWorld)
		World::Remove(*this);

	inWorld = false;
}

WorldObject::ObjectType WorldObject::GetObjectType() const
{
	return type;
}
void WorldObject::Draw(Screen& target) const
{
	SDL_Surface* surface = target.GetSurface();

	SDL_Rect rect;
	rect.w = maxBounds.x - minBounds.x;
	rect.h = maxBounds.y - minBounds.y;
	rect.x = minBounds.x;
	rect.y = minBounds.y;

	if(SDL_FillRect(surface, &rect, color.GetRGBMap(surface)) == -1)
	{
		string error = "Error drawing to screen: ";
		error += SDL_GetError();
		logger.Fatal(error.c_str());
	}
}

Point WorldObject::GetMinBounds() const
{
	return minBounds;
}
Point WorldObject::GetMaxBounds() const
{
	return maxBounds;
}
Color24 WorldObject::GetColor() const
{
	return color;
}
}
