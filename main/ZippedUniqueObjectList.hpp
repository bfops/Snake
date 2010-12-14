#pragma once

#include "UniqueObjectList.hpp"

// do _stuffToDo_ while _obj_'s internal mutexes are locked
#define DOLOCKEDZ(obj, stuffToDo) \
	DOLOCKED(obj.graphics.mutex, \
		DOLOCKED(obj.physics.mutex, \
			stuffToDo \
		) \
	)

struct ZippedUniqueObjectList
{
	// apply _func_ to _graphics_ and _physics_
#define DOBOTH(func) graphics.func; physics.func;
	UniqueObjectList graphics, physics;

	inline void Add(WorldObject& obj)
	{
		DOBOTH(Add(obj))
	}

	template<typename Iter>
	inline void AddRange(Iter begin, Iter end)
	{
		DOBOTH(AddRange(begin, end))
	}

	inline void Remove(WorldObject& obj)
	{
		DOBOTH(Remove(obj))
	}

	template<typename Iter>
	inline void RemoveRange(Iter begin, Iter end)
	{
		DOBOTH(RemoveRange(begin, end))
	}

#undef DOBOTH
};
