#include "World.hpp"
#include "WorldObject.hpp"

#include <vector>
#include <boost/bind/bind.hpp>

#include "custom_algorithm.hpp"
#include "Logger.hpp"

using namespace std;

namespace World {
namespace {
namespace GraphicsWorld {
namespace {
typedef vector<const WorldObject*> GraphicsObjectList;
GraphicsObjectList objects;
}

void Add(const WorldObject& object)
{
	if(find(objects.begin(), objects.end(), &object) != objects.end())
		assert(!"Adding pre-added thing!");

	objects.push_back(&object);
}
void Remove(const WorldObject& object)
{
	if(!unordered_find_and_remove(objects, &object))
		// TODO: FIX TO LOGGER!
	assert(!"Removed invalid thing");
}

// TODO: just move the damn screen in?
void Update(Screen& target)
{
	target.Clear();

	for(GraphicsObjectList::iterator i = objects.begin(), end = objects.end(); i != end; ++i)
		(*i)->Draw(target);

	target.Update();
}
}

namespace PhysicsWorld {
namespace {
Logger::Handle logger = Logger::RequestHandle("PhysicsWorld");
// URGENT TODO: nicer abstraction.
// Allocate a PhysicsGroup, and then you can add
// things already in the world to that group
typedef std::vector<WorldObject*> PhysicsGroup;
typedef vector<PhysicsGroup> PhysicsObjectList;
PhysicsObjectList groups;
}

static inline bool IsWithinBounds(int obj1Location, int obj2Location, unsigned int dimension)
{
	return((obj1Location >= obj2Location) && (obj1Location < (int)(obj2Location + dimension)));
}

static inline bool IsLeftWithinBounds(const WorldObject* obj1, const WorldObject* obj2)
{
	return IsWithinBounds(obj1->location.x, obj2->location.x, obj2->width);
}

static inline bool IsTopWithinBounds(const WorldObject* obj1, const WorldObject* obj2)
{
	return IsWithinBounds(obj1->location.y, obj2->location.y, obj2->height);
}

static inline bool IsCollide(const WorldObject* obj1, const WorldObject* obj2)
{
	return (
		(IsTopWithinBounds(obj1, obj2) || IsTopWithinBounds(obj2, obj1))
		&& (IsLeftWithinBounds(obj1, obj2) || IsLeftWithinBounds(obj2, obj1))
	);
}

static inline bool object_found(PhysicsGroup& group, const WorldObject* obj)
{
	return find(group.begin(), group.end(), obj) != group.end();
}

static inline bool object_exists(const WorldObject* obj)
{
	return any(groups.begin(), groups.end(), boost::bind(object_found, _1, obj));
}

static inline bool any_objects_exist(const PhysicsGroup& group)
{
	return any(group.begin(), group.end(), object_exists);
}

void Add(WorldObject& obj)
{
	assert(!object_exists(&obj));

	PhysicsGroup group;
	group.push_back(&obj);
	groups.push_back(group);
	logger.Debug(boost::format("Type %1% object %2% added") % obj.GetObjectType() % &obj);
}

void Add(PhysicsGroup& group)
{
	assert(!any_objects_exist(group));

	groups.push_back(group);

	// TODO: output group members
	logger.Debug("Object group added!");
}

void Remove(WorldObject& obj)
{
	for(PhysicsObjectList::iterator group = groups.begin(), end = groups.end(); group != end; ++group)
	{
		if(unordered_find_and_remove(*group, &obj))
		{
			if(group->size() == 0)
				unordered_remove(groups, group);

			return logger.Debug(boost::format("Type %1% object %2% removed") % obj.GetObjectType() % &obj);
		}
	}
	logger.Debug("Invalid object specified");
}

void RemoveGroup(WorldObject& obj)
{
	for(PhysicsObjectList::iterator group = groups.begin(), end = groups.end(); group != end; ++group)
		if(object_found(*group, &obj))
			return unordered_remove(groups, group);

	logger.Debug("Invalid group specified");
}

static inline void handle_potential_collision(Logger::Handle logger, WorldObject* o1, WorldObject* o2)
{
	if(IsCollide(o1, o2))
	{
		o1->CollisionHandler(*o2);
		o2->CollisionHandler(*o1);
	}
}

static inline void collide_with_other_groups(PhysicsObjectList::iterator startGroup, WorldObject* collider)
{
	for(PhysicsObjectList::iterator group = startGroup + 1, lastGroup = groups.end(); group != lastGroup; ++group)
		std::for_each(group->begin(), group->end(), boost::bind(handle_potential_collision, logger, collider, _1));
}

void Update()
{
	for(PhysicsObjectList::iterator group = groups.begin(), lastGroup = groups.end() - 1; group != lastGroup; ++group)
		std::for_each(group->begin(), group->end(), boost::bind(collide_with_other_groups, group, _1));
}

}
}

void Update(Screen& target)
{
	PhysicsWorld::Update();
	GraphicsWorld::Update(target);
}

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
}
