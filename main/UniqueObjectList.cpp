#include "ZippedUniqueObjectList.hpp"

#include "Common.hpp"
#include "custom_algorithm.hpp"
#include "Logger.hpp"

static Logger::Handle logger = Logger::RequestHandle("UniqueObjectList");

static inline bool object_exists(const UniqueObjectList::CollectionType& list, const WorldObject* val)
{
	return in(list.begin(), list.end(), val);
}

void UniqueObjectList::add(WorldObject& pObj)
{
	Lock();

	DEBUGLOGIF(object_exists(objects, &pObj), logger, boost::format("Object %1% already exists.") % &pObj)
	objects.push_back(&pObj);

	Unlock();
}

void UniqueObjectList::remove(WorldObject& pObj)
{
	Lock();

	DEBUGLOGIF(!object_exists(objects, &pObj), logger, boost::format("Object %1% does not exist.") % &pObj)
	unordered_find_and_remove(objects, &pObj);

	Unlock();
}
