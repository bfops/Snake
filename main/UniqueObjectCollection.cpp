#include "UniqueObjectCollection.hpp"

#include "custom_algorithm.hpp"
#include "Logger.hpp"

static inline bool object_exists(const UniqueObjectCollection::CollectionType& list, const WorldObject* const val)
{
	return in(list.begin(), list.end(), val);
}

UniqueObjectCollection::UniqueObjectCollection()
{
}

UniqueObjectCollection::UniqueObjectCollection(const UniqueObjectCollection& obj) :
	objects(obj.objects)
{
}

UniqueObjectCollection::iterator UniqueObjectCollection::begin()
{
	return objects.begin();
}

UniqueObjectCollection::const_iterator UniqueObjectCollection::begin() const
{
	return objects.begin();
}

UniqueObjectCollection::iterator UniqueObjectCollection::end()
{
	return objects.end();
}

UniqueObjectCollection::const_iterator UniqueObjectCollection::end() const
{
	return objects.end();
}

void UniqueObjectCollection::Add(WorldObject& obj)
{
	if(object_exists(objects, &obj))
		Logger::Debug(boost::format("Object %1% already exists.") % &obj);

	objects.push_back(&obj);
}

void UniqueObjectCollection::Remove(WorldObject& obj)
{
	if(!object_exists(objects, &obj))
		Logger::Debug(boost::format("Object %1% does not exist.") % &obj);

	unordered_find_and_remove(objects, &obj);
}
