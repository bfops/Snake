#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

#ifdef MSVC
#pragma warning(pop)
#endif

#define DOLOCKEDU(obj, thingsToDo) { \
	UniqueObjectList::Lock uniqueLock(obj); \
	thingsToDo \
}

class WorldObject;

class UniqueObjectList
{
public:
	typedef std::vector<WorldObject*> CollectionType;
	typedef CollectionType::iterator iterator;

private:
	CollectionType objects;

public:
	class Lock
	{
	private:
		boost::mutex& m;

	public:
		Lock(UniqueObjectList& _m) :
			m(_m.mutex)
		{
			m.lock();
		}

		~Lock()
		{
			m.unlock();
		}
	};

	boost::mutex mutex;

	UniqueObjectList()
	{
	}

	UniqueObjectList(const UniqueObjectList& obj) :
		objects(obj.objects)
	{
	}

	void Add(WorldObject&);

	template<typename Iter>
	inline void AddRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::Add, this, _1));
	}

	void Remove(WorldObject&);

	template<typename Iter>
	inline void RemoveRange(Iter begin, Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::Remove, this, _1));
	}

	inline iterator begin()
	{
		return objects.begin();
	}

	inline iterator end()
	{
		return objects.end();
	}
};
