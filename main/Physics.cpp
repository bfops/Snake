#include "Physics.hpp"

#include "Bounds.hpp"
#include "collision.h"
#include "GameWorld.hpp"
#include "WorldObject.hpp"
#include "ZippedUniqueObjectList.hpp"

#include <algorithm>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

namespace Physics
{
	static inline CollidableObject world_to_collidable_object(const WorldObject* w)
	{
		CollidableObject ret;

		const Bounds bounds = w->GetBounds();
		ret.min.x = bounds.min.x;
		ret.min.y = bounds.min.y;
		ret.max.x = bounds.max.x;
		ret.max.y = bounds.max.y;

		return ret;
	}

	static void handle_potential_collision(GameWorld* const world, ZippedUniqueObjectList& objects,
										   WorldObject* const o1, WorldObject* const o2)
	{
		CollidableObject c1 = world_to_collidable_object(o1);
		CollidableObject c2 = world_to_collidable_object(o2);

		if(does_collide(&c1, &c2))
			world->CollisionHandler(objects, *o1, *o2);
	}

	static inline void collide_with_subsequent_objects(
		GameWorld* world, ZippedUniqueObjectList& worldObjects,
		UniqueObjectList::iterator collider, UniqueObjectList::iterator end)
	{
		for_each(collider + 1, end, bind(&handle_potential_collision, world, ref(worldObjects), *collider, _1));
	}

	void Update(GameWorld& world, ZippedUniqueObjectList& worldObjects)
	{
		ZippedUniqueObjectList original(worldObjects);
		// don't try the last gameObject, since all have been checked against it
		for(UniqueObjectList::iterator collider = original.physics.begin(), end = original.physics.end() - 1;
			collider != end; ++collider)
			collide_with_subsequent_objects(&world, worldObjects, collider, original.physics.end());
	}
}
