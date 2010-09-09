#pragma once

#include <vector>

#include "WorldObject.hpp"

namespace PhysicsWorld
{
	// TODO: nicer abstraction
	typedef std::vector<WorldObject*> PhysicsGroup;

	void Add(WorldObject&);
	void Add(PhysicsGroup&);
	void Remove(WorldObject&);
	// Remove the group which contains _containedElement_
	void RemoveGroup(WorldObject& containedElement);
	void Update();
}
