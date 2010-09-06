#include <cassert>
#include <list>

#include "Physics.hpp"

#include <algorithm>

using namespace std;

namespace PhysicsWorld
{
	namespace
	{
		typedef vector<WorldObject*> PhysicsObjectList;
		PhysicsObjectList objects;
	}
	static bool IsCollide(const WorldObject&, const WorldObject&);
	static bool IsWithinBounds(int obj1Location, int obj2Location, unsigned int obj2Dimension);
	static bool IsLeftWithinBounds(const WorldObject&, const WorldObject&);
	static bool IsTopWithinBounds(const WorldObject&, const WorldObject&);

	static bool object_exists(const WorldObject& obj)
	{
		return std::find(objects.begin(), objects.end(), &obj) != objects.end();
	}
	void AddObject(WorldObject& obj)
	{
		assert(!object_exists(obj));
		objects.push_back(&obj);
	}
	void RemoveObject(WorldObject& obj)
	{
		for(PhysicsObjectList::iterator i = objects.begin(), end = objects.end(); i != end; ++i)
		{
			if(*i == &obj)
			{
				// sexy removal hack.
				// it's linear-time either way,
				// but this is faster in practice
				swap(*i, *objects.rbegin());
				objects.pop_back();

				return;
			}
		}

		// TODO: throw errors at this point
	}
	void Update()
	{
		// TODO: only check those that have moved
		for(PhysicsObjectList::iterator collider = objects.begin(), end = objects.end(); collider != end; ++collider)
		{
			PhysicsObjectList::iterator collidee = collider;

			for(++collidee; collidee != end; ++collidee)
			{
				WorldObject* c1 = *collider;
				WorldObject* c2 = *collidee;

				if(IsCollide(*c1, *c2))
				{
					c1->CollisionHandler(*c2);
					c2->CollisionHandler(*c1);
				}
			}
		}
	}

	// TODO: try with member pointers
	static bool IsWithinBounds(int obj1Location, int obj2Location, unsigned int dimension)
	{
		return((obj1Location >= obj2Location) && (obj1Location <= (int)(obj2Location + dimension)));
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
