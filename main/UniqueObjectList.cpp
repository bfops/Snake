#include "UniqueObjectList.hpp"

#include "custom_algorithm.hpp"
#include "Logger.hpp"

static inline bool object_exists(const UniqueObjectList::CollectionType& list, const WorldObject* const val)
{
	return in(list.begin(), list.end(), val);
}

UniqueObjectList::UniqueObjectList()
{
}

UniqueObjectList::UniqueObjectList(const UniqueObjectList& obj) :
	objects(obj.objects)
{
}

UniqueObjectList::iterator UniqueObjectList::begin()
{
	return objects.begin();
}

UniqueObjectList::const_iterator UniqueObjectList::begin() const
{
	return objects.begin();
}

UniqueObjectList::iterator UniqueObjectList::end()
{
	return objects.end();
}

UniqueObjectList::const_iterator UniqueObjectList::end() const
{
	return objects.end();
}

void UniqueObjectList::Add(WorldObject& obj)
{
	if(object_exists(objects, &obj))
		Logger::Debug(boost::format("Object %1% already exists.") % &obj);

	objects.push_back(&obj);
}

void UniqueObjectList::Remove(WorldObject& obj)
{
	if(!object_exists(objects, &obj))
		Logger::Debug(boost::format("Object %1% does not exist.") % &obj);

	unordered_find_and_remove(objects, &obj);
}
