#pragma once

#include "WorldObject.hpp"

struct Color24;

// spawnable objects should inherit from this (e.g. food, mines)
class Spawn : public WorldObject
{
public:
	Spawn::Spawn(ObjectType spawnType, Point location, unsigned short size, Color24);
	virtual ~Spawn();

	void ShrinkDown(unsigned short newSize);
};
