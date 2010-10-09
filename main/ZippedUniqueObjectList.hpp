#pragma once

#include "UniqueObjectList.hpp"

struct ZippedUniqueObjectList
{
	UniqueObjectList graphics, physics;

	inline void add(WorldObject& obj)
	{
		graphics.add(obj);
		physics.add(obj);
	}
	template<typename Iter>
	inline void addRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::add, &graphics, _1));
		std::for_each(begin, end, boost::bind(&UniqueObjectList::add, &physics, _1));
	}
	inline void remove(WorldObject& obj)
	{
		graphics.remove(obj);
		physics.remove(obj);
	}
	template<typename Iter>
	inline void removeRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::remove, &graphics, _1));
		std::for_each(begin, end, boost::bind(&UniqueObjectList::remove, &physics, _1));
	}
};
