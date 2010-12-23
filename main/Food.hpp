#pragma once

#include "Config.hpp"
#include "Spawn.hpp"

class Sentinel;

class Food : public Spawn
{
public:
	Food(const Sentinel& prototype);

	void CollisionHandler(WorldObject&) const;
};
