#pragma once

#include "UniqueObjectList.hpp"

#define DOLOCKEDZ(obj, thingsToDo) { \
	ZippedUniqueObjectList::Lock uniqueLock(obj); \
	thingsToDo \
}

struct ZippedUniqueObjectList
{
#define DOBOTH(func) graphics.func; physics.func;
	
	class Lock
	{
	private:
		UniqueObjectList::Lock l1, l2;

	public:
		Lock(ZippedUniqueObjectList& l) :
			l1(l.graphics), l2(l.physics)
		{
		}
	};

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
