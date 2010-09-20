#pragma once

#include <vector>

#include "WorldObject.hpp"

class UniqueObjectList
{
public:
	typedef std::vector<WorldObject*> List;
	typedef List::iterator Iterator;

private:
	List objects;

public:
	void add(WorldObject*);
	void remove(WorldObject*);

	Iterator begin();
	Iterator end();
};
