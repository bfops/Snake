#include "UniqueObjectList.hpp"

#include "Common.hpp"
#include "custom_algorithm.hpp"
#include "Logger.hpp"

namespace {
Logger::Handle logger = Logger::RequestHandle("UniqueObjectList");

bool object_exists(const UniqueObjectList::List& list, const WorldObject* val)
{
	return in(list.begin(), list.end(), val);
}
}

void UniqueObjectList::add(WorldObject& pObj)
{
	DEBUGLOGIF(object_exists(objects, &pObj), logger, boost::format("Object %1% already exists.") % &pObj)

	objects.push_back(&pObj);
}

void UniqueObjectList::remove(WorldObject& pObj)
{
	DEBUGLOGIF(!object_exists(objects, &pObj), logger, boost::format("Object %1% does not exist.") % &pObj)

	unordered_find_and_remove(objects, &pObj);
}

UniqueObjectList::iterator UniqueObjectList::begin()
{
	return objects.begin();
}

UniqueObjectList::iterator UniqueObjectList::end()
{
	return objects.end();
}

