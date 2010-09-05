#include <cassert>
#include <list>

#include "Physics.hpp"

using namespace std;

namespace PhysicsWorld
{
	namespace
	{
		typedef list<WorldObject*> PhysicsObjectList;
		list<WorldObject*> objects;

	}
	static bool IsCollide(const WorldObject&, const WorldObject&);
	static bool IsWithinBounds(const WorldObject&, const WorldObject&, int Point::* element, size_t WorldObject::* dimension);
	static bool IsLeftWithinBounds(const WorldObject&, const WorldObject&);
	static bool IsTopWithinBounds(const WorldObject&, const WorldObject&);

	void AddObject(WorldObject& obj)
	{
		// this should get optimized out in released build
		for(PhysicsObjectList::iterator i = objects.begin(); i != objects.end(); ++i)
		{
			assert(*i != &obj);
		}
		objects.push_back(&obj);
	}
	void RemoveObject(WorldObject& obj)
	{
		objects.remove(&obj);
	}
	void Update()
	{
		// TODO: only check those that have moved
		for(PhysicsObjectList::iterator i = objects.begin(); i != objects.end(); ++i)
		{
			PhysicsObjectList::iterator j = i;
			++j;
			for(; j != objects.end(); ++j)
			{
				if(IsCollide(**i, **j))
				{
					(*i)->CollisionHandler(**j);
					(*j)->CollisionHandler(**i);
				}
			}
		}
	}

	static bool IsWithinBounds(const WorldObject& obj1, const WorldObject& obj2, int Point::* mbr, size_t WorldObject::* dmsn)
	{
		return((obj1.location.*mbr >= obj2.location.*mbr) && (obj1.location.*mbr <= (int)(obj2.location.*mbr + obj2.*dmsn)));
	}
	static bool IsLeftWithinBounds(const WorldObject& obj1, const WorldObject& obj2)
	{
		return IsWithinBounds(obj1, obj2, &Point::x, &WorldObject::width);
	}
	static bool IsTopWithinBounds(const WorldObject& obj1, const WorldObject& obj2)
	{
		return IsWithinBounds(obj1, obj2, &Point::y, &WorldObject::height);
	}
	static bool IsCollide(const WorldObject& obj1, const WorldObject& obj2)
	{
		return (
			(IsTopWithinBounds(obj1, obj2) || IsTopWithinBounds(obj2, obj1))
			&& (IsLeftWithinBounds(obj1, obj2) || IsLeftWithinBounds(obj2, obj1))
		);
	}
}
