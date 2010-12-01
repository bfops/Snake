#pragma once

#include "UniqueObjectList.hpp"

#define DOLOCKEDZ(obj, stuffToDo) DOLOCKED(obj.graphics.mutex, \
	DOLOCKED(obj.physics.mutex, \
		stuffToDo \
	) \
)

struct ZippedUniqueObjectList
{
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
