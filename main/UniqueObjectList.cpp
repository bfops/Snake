#include "ZippedUniqueObjectList.hpp"

#include "Common.hpp"
#include "custom_algorithm.hpp"
#include "Logger.hpp"

static inline bool object_exists(const UniqueObjectList::CollectionType& list, const WorldObject* val)
{
	return in(list.begin(), list.end(), val);
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
