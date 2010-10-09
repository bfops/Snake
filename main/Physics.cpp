#include "Physics.hpp"

#include "Bounds.hpp"
#include "collision.h"
#include "GameWorld.hpp"
#include "UniqueObjectList.hpp"
#include "WorldObject.hpp"

#include <algorithm>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

namespace Physics
{
	static inline CollidableObject world_to_collidable_object(const WorldObject* w)
	{
		CollidableObject ret;

		Bounds bounds = w->GetBounds();
		ret.min.x = bounds.min.x;
		ret.min.y = bounds.min.y;
		ret.max.x = bounds.max.x;
		ret.max.y = bounds.max.y;

		return ret;
	}

	static inline void handle_potential_collision(GameWorld* world, WorldObject* o1, WorldObject* o2)
	{
		CollidableObject c1 = world_to_collidable_object(o1);
		CollidableObject c2 = world_to_collidable_object(o2);

		if(does_collide(&c1, &c2))
		{
			o1->CollisionHandler(*o2);
			o2->CollisionHandler(*o1);
			world->CollisionNotify(o1->GetObjectType(), o2->GetObjectType());
		}
	}

	static inline void collide_with_subsequent_objects(GameWorld* world, UniqueObjectList::iterator collider, UniqueObjectList::iterator end)
	{
		for_each(collider + 1, end, bind(&handle_potential_collision, world, *collider, _1));
	}

	void Update(GameWorld& world, UniqueObjectList& physicsObjects)
	{
		// don't try the last gameObject, since all have been checked against it
		for(UniqueObjectList::iterator collider = physicsObjects.begin(), end = physicsObjects.end() - 1; collider != end; ++collider)
			collide_with_subsequent_objects(&world, collider, physicsObjects.end());
	}
}
