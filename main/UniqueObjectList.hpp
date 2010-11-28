#pragma once

#ifdef MSVC
#pragma warning( push, 0 )
#endif

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

#ifdef MSVC
#pragma warning( pop )
#endif

class WorldObject;

class UniqueObjectList
{
public:
	typedef std::vector<WorldObject*> CollectionType;
	typedef CollectionType::iterator iterator;

private:
	boost::mutex mutex;
	CollectionType objects;

public:
	UniqueObjectList()
	{
	}

	UniqueObjectList(const UniqueObjectList& obj) :
		objects(obj.objects)
	{
	}

	void add(WorldObject&);

	template<typename Iter>
	inline void addRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::add, this, _1));
	}

	void remove(WorldObject&);

	template<typename Iter>
	inline void removeRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::remove, this, _1));
	}

	inline iterator begin()
	{
		return objects.begin();
	}

	inline iterator end()
	{
		return objects.end();
	}

	inline void Lock()
	{
		mutex.lock();
	}

	inline void Unlock()
	{
		mutex.unlock();
	}
};
