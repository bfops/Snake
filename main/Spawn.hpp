#pragma once

#include "WorldObject.hpp"

struct Color24;
class Sentinel;

// spawnable objects should inherit from this (e.g. food, mines)
class Spawn : public WorldObject
{
public:
	Spawn(ObjectType spawnType, const Sentinel& prototype, unsigned short spawnSize, Color24 spawnColor);
	virtual ~Spawn();
};
