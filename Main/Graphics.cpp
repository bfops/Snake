#include "Graphics.hpp"

#include <cassert>
#include <vector>

#include "custom_algorithm.hpp"

using namespace std;

namespace GraphicsWorld {

namespace {
typedef vector<const WorldObject*> GraphicsObjectList;
GraphicsObjectList objects;
}

void Add(const WorldObject& object)
{
	if(find(objects.begin(), objects.end(), &object) != objects.end())
		assert(!"Adding pre-added thing!");

	objects.push_back(&object);
}
void Remove(const WorldObject& object)
{
	if(!unordered_find_and_remove(objects, &object))
		// TODO: FIX TO LOGGER!
		assert(!"Removed invalid thing");
}

// TODO: just move the damn screen in?
void Update(Screen& target)
{
	target.Clear();

	for(GraphicsObjectList::iterator i = objects.begin(), end = objects.end(); i != end; ++i)
		(*i)->Draw(target);

	target.Update();
}

}