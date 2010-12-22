#include "Physics.hpp"

#include "Bounds.hpp"
#include "Common.hpp"
#include "collision.h"
#include "GameWorld.hpp"
#include "UniqueObjectList.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <algorithm>
#include <boost/bind.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

namespace Physics
{
	static inline ObjectBounds get_world_object_bounds(const WorldObject* const w)
	{
		ObjectBounds ret;

		const Bounds bounds = w->GetBounds();
		ret.min.x = bounds.min.x;
		ret.min.y = bounds.min.y;
		ret.max.x = bounds.max.x;
		ret.max.y = bounds.max.y;

		return ret;
	}

	static inline bool does_collide(const WorldObject& o1, const WorldObject& o2)
	{
		DOLOCKED(o1.mutex,
			const ObjectBounds c1 = get_world_object_bounds(&o1);
		)
		DOLOCKED(o2.mutex,
			const ObjectBounds c2 = get_world_object_bounds(&o2);
		)

		return does_collide(&c1, &c2) != 0;
	}
	
	static void handle_potential_collision(GameWorld* const world, WorldObject* const o1,
		WorldObject* const o2)
	{
		DOLOCKED(o1->mutex,
			DOLOCKED(o2->mutex,
				if(does_collide(*o1, *o2))
					world->CollisionHandler(*o1, *o2);
			)
		)
	}

	static inline void collide_with_subsequent_objects(GameWorld* const world,
		const UniqueObjectList::const_iterator collider, const UniqueObjectList::const_iterator end)
	{
		for_each(collider + 1, end, bind(&handle_potential_collision, world, *collider, _1));
	}

	void Update(GameWorld& world, const UniqueObjectList& realPhysicsObjects)
	{
		if(realPhysicsObjects.begin() == realPhysicsObjects.end())
			return;

		const UniqueObjectList physicsObjects(realPhysicsObjects);

		// don't try the last gameObject, since all have been checked against it
		for(UniqueObjectList::const_iterator collider = physicsObjects.begin(),
			end = physicsObjects.end() - 1; collider != end; ++collider)
			collide_with_subsequent_objects(&world, collider, physicsObjects.end());
	}

	bool AnyCollide(const WorldObject& obj, const UniqueObjectList& physicsObjects)
	{
		DOLOCKED(physicsObjects.mutex,
			for(UniqueObjectList::const_iterator collider = physicsObjects.begin(),
				end = physicsObjects.end(); collider != end; ++collider)
			{
				if(does_collide(obj, **collider))
				{
					physicsObjects.mutex.Unlock();
					return true;
				}
			}
		)

		return false;
	}
}
