#include <boost/bind/bind.hpp>
#include <cassert>
#include <list>

#include "custom_algorithm.hpp"

#include "Common.hpp"
#include "Logger.hpp"
#include "Physics.hpp"

using namespace std;

static Logger::Handle logger = Logger::RequestHandle("PhysicsWorld");

namespace PhysicsWorld {

namespace {
typedef vector<PhysicsGroup> PhysicsObjectList;
PhysicsObjectList groups;
}

static bool IsCollide(const WorldObject*, const WorldObject*);
static bool IsWithinBounds(int obj1Location, int obj2Location, unsigned int obj2Dimension);
static bool IsLeftWithinBounds(const WorldObject*, const WorldObject*);
static bool IsTopWithinBounds(const WorldObject*, const WorldObject*);

static bool object_found(PhysicsGroup& group, const WorldObject* obj)
{
	return find(group.begin(), group.end(), obj) != group.end();
}

static bool object_exists(const WorldObject* obj)
{
	return any(groups.begin(), groups.end(), boost::bind(object_found, _1, obj));
}

static bool any_objects_exist(const PhysicsGroup& group)
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

static void handle_potential_collision(Logger::Handle logger, WorldObject* o1, WorldObject* o2)
{
	if(IsCollide(o1, o2))
	{
		o1->CollisionHandler(*o2);
		o2->CollisionHandler(*o1);
	}
}

static void collide_with_other_groups(PhysicsObjectList::iterator startGroup, WorldObject* collider)
{
	for(PhysicsObjectList::iterator group = startGroup + 1, lastGroup = groups.end(); group != lastGroup; ++group)
		std::for_each(group->begin(), group->end(), boost::bind(handle_potential_collision, logger, collider, _1));
}

void Update()
{
	for(PhysicsObjectList::iterator group = groups.begin(), lastGroup = groups.end() - 1; group != lastGroup; ++group)
		std::for_each(group->begin(), group->end(), boost::bind(collide_with_other_groups, group, _1));
}

static bool IsWithinBounds(int obj1Location, int obj2Location, unsigned int dimension)
{
	return((obj1Location >= obj2Location) && (obj1Location < (int)(obj2Location + dimension)));
}
static bool IsLeftWithinBounds(const WorldObject* obj1, const WorldObject* obj2)
{
	return IsWithinBounds(obj1->location.x, obj2->location.x, obj2->width);
}
static bool IsTopWithinBounds(const WorldObject* obj1, const WorldObject* obj2)
{
	return IsWithinBounds(obj1->location.y, obj2->location.y, obj2->height);
}
static bool IsCollide(const WorldObject* obj1, const WorldObject* obj2)
{
	return (
		(IsTopWithinBounds(obj1, obj2) || IsTopWithinBounds(obj2, obj1))
		&& (IsLeftWithinBounds(obj1, obj2) || IsLeftWithinBounds(obj2, obj1))
	);
}
}
