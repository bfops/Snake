#pragma once

#include <algorithm>
#include <boost/bind.hpp>
#include <vector>

class WorldObject;

// TODO: UniqueObjectList zipper
class UniqueObjectList
{
public:
	typedef std::vector<WorldObject*> List;
	typedef List::iterator iterator;

private:
	List objects;

public:
	void add(WorldObject&);
	template<typename Iter>
	void addRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::add, this, _1));
	}
	void remove(WorldObject&);
	template<typename Iter>
	void removeRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::remove, this, _1));
	}
	void clear();

	iterator begin();
	iterator end();
};
