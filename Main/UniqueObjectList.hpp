#pragma once

#include <vector>

class WorldObject;

class UniqueObjectList
{
public:
	typedef std::vector<WorldObject*> List;
	typedef List::iterator iterator;

private:
	List objects;

public:
	void add(WorldObject&);
	void remove(WorldObject&);
	void clear();

	iterator begin();
	iterator end();
};
