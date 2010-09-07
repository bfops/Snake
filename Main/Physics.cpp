#include <cassert>
#include <cstdio>
#include <list>

#include "custom_algorithm.hpp"
#include "Physics.hpp"

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

		#ifndef NDEBUG
		printf("Type %u object added: %p12\n", obj.GetObjectType(), (void*)&obj);
		#endif
		objects.push_back(&obj);
	}
	void RemoveObject(WorldObject& obj)
	{
		#ifndef NDEBUG
		printf("Type %u object removed: %p12\n", obj.GetObjectType(), (void*)&obj);
		#endif

		assert(object_exists(obj));
		unordered_find_and_remove(objects, &obj);
	}
	void Update()
	{
		// TODO: only check those that have moved
		for(PhysicsObjectList::iterator collider = objects.begin(), end = objects.end(); collider != end; ++collider)
		{
			PhysicsObjectList::iterator collidee = collider;

			for(++collidee; collidee != end; ++collidee)
			{
				if(IsCollide(**collider, **collidee))
				{
					#ifndef NDEBUG
					if((*collider)->GetObjectType() != 1 || (*collidee)->GetObjectType() != 1)
						printf("Collision between %p12 [(%i,%i) %ux%u] and %p12 [(%i,%i) %ux%u]\n",
							(void*)*collider, (*collider)->location.x, (*collider)->location.y, (*collider)->width, (*collider)->height,
							(void*)*collidee, (*collidee)->location.x, (*collidee)->location.y, (*collidee)->width, (*collidee)->height
						);
					#endif
					(*collider)->CollisionHandler(**collidee);
					(*collidee)->CollisionHandler(**collider);
				}
			}
		}
	}

	// TODO: try with member pointers
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
