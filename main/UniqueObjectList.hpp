#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/bind.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <vector>

#ifdef MSVC
#pragma warning(pop)
#endif

class WorldObject;

// maintain a unique list of WorldObject*s, with
// debug-only assertations of uniqueness
class UniqueObjectList
{
public:
	typedef std::vector<WorldObject*> CollectionType;
	typedef CollectionType::iterator iterator;
	typedef CollectionType::const_iterator const_iterator;

private:
	CollectionType objects;

public:
	boost::recursive_mutex mutex;

	UniqueObjectList()
	{
	}

	UniqueObjectList(const UniqueObjectList& obj) :
		objects(obj.objects)
	{
	}

	void Add(WorldObject&);

	// add the objects from _begin_ to _end_ to this list
	template<typename Iter>
	inline void AddRange(const Iter begin, const Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::Add, this, _1));
	}

	void Remove(WorldObject&);

	// remove the objects from _begin_ to _end_ from this list
	template<typename Iter>
	inline void RemoveRange(const Iter begin, const Iter end)
	{
		std::for_each(begin, end, boost::bind(&UniqueObjectList::Remove, this, _1));
	}

	inline iterator begin()
	{
		return objects.begin();
	}

	inline const_iterator begin() const
	{
		return objects.begin();
	}

	inline iterator end()
	{
		return objects.end();
	}

	inline const_iterator end() const
	{
		return objects.end();
	}
};
