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

void UniqueObjectList::add(WorldObject* pObj)
{
	DEBUGLOG(object_exists(objects, pObj), logger, "Object pObj already exists.")

	objects.push_back(pObj);
}

void UniqueObjectList::remove(WorldObject* pObj)
{
	DEBUGLOG(!object_exists(objects, pObj), logger, "Object pObj does not exist.")

	unordered_find_and_remove(objects, pObj);
}

UniqueObjectList::Iterator UniqueObjectList::begin()
{
	return objects.begin();
}

UniqueObjectList::Iterator UniqueObjectList::end()
{
	return objects.end();
}

