#pragma once

#include <vector>

#include "WorldObject.hpp"

namespace PhysicsWorld
{
	// URGENT TODO: nicer abstraction.
	// Allocate a PhysicsGroup, and then you can add
	// things already in the world to that group
	typedef std::vector<WorldObject*> PhysicsGroup;

	void Add(WorldObject&);
	void Add(PhysicsGroup&);
	void Remove(WorldObject&);
	// Remove the group which contains _containedElement_
	void RemoveGroup(WorldObject& containedElement);
	void Update();
}
