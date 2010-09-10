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

static bool IsCollide(const WorldObject&, const WorldObject&);
static bool IsWithinBounds(int obj1Location, int obj2Location, unsigned int obj2Dimension);
static bool IsLeftWithinBounds(const WorldObject&, const WorldObject&);
static bool IsTopWithinBounds(const WorldObject&, const WorldObject&);

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
	logger.Debug(boost::format("Type %1% object %1% added") % obj.GetObjectType() % (void*)&obj);
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

			logger.Debug(boost::format("Type %1% object %2% removed") % obj.GetObjectType() % (void*)&obj);
			return;
		}
	}
	logger.Debug("Invalid object specified");
}
void RemoveGroup(WorldObject& obj)
{
	for(PhysicsObjectList::iterator group = groups.begin(), end = groups.end(); group != end; ++group)
	{
		if(find(group->begin(), group->end(), &obj) != group->end())
		{
			unordered_remove(groups, group);
			return;
		}
	}
	logger.Debug("Invalid group specified");
}
static void collide_with_other_groups(PhysicsObjectList::iterator startGroup, PhysicsGroup::iterator collider)
{
	for(PhysicsObjectList::iterator group = startGroup + 1, lastGroup = groups.end(); group != lastGroup; ++group)
	{
		for(PhysicsGroup::iterator collidee = group->begin(), end = group->end(); collidee != end; ++collidee)
		{
			if(IsCollide(**collider, **collidee))
			{
				logger.Debug(
					boost::format("Collision between %1% [(%2%, %3%) %4%x%5%] and %6% [(%7%, %8%) %9%x%10%]")
					% *collider % (*collider)->location.x % (*collider)->location.y % (*collider)->width % (*collider)->height
					% *collidee % (*collidee)->location.x % (*collidee)->location.y % (*collidee)->width % (*collidee)->height
				);

				(*collider)->CollisionHandler(**collidee);
				(*collidee)->CollisionHandler(**collider);
			}
		}
	}
}
void Update()
{
	for(PhysicsObjectList::iterator group = groups.begin(), lastGroup = groups.end() - 1; group != lastGroup; ++group)
	{
		for(PhysicsGroup::iterator collider = group->begin(), end = group->end(); collider != end; ++collider)
		{
			collide_with_other_groups(group, collider);
		}
	}
}

static bool IsWithinBounds(int obj1Location, int obj2Location, unsigned int dimension)
{
	return((obj1Location >= obj2Location) && (obj1Location < (int)(obj2Location + dimension)));
}
static bool IsLeftWithinBounds(const WorldObject& obj1, const WorldObject& obj2)
{
	return IsWithinBounds(obj1.location.x, obj2.location.x, obj2.width);
}
static bool IsTopWithinBounds(const WorldObject& obj1, const WorldObject& obj2)
{
	return IsWithinBounds(obj1.location.y, obj2.location.y, obj2.height);
}
static bool IsCollide(const WorldObject& obj1, const WorldObject& obj2)
{
	return (
		(IsTopWithinBounds(obj1, obj2) || IsTopWithinBounds(obj2, obj1))
		&& (IsLeftWithinBounds(obj1, obj2) || IsLeftWithinBounds(obj2, obj1))
	);
}
}
