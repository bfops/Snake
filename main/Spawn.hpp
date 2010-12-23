#pragma once

#include "Config.hpp"
#include "WorldObject.hpp"

struct Color24;
class Sentinel;

// spawnable objects should inherit from this (e.g. food, mines)
class Spawn : public WorldObject
{
protected:
	const Config::SpawnsData::SpawnData* spawnData;

public:
	Spawn(ObjectType spawnType, const Sentinel& prototype);
	virtual ~Spawn();

	const Config::SpawnsData::SpawnData& GetSpawnData() const;
};
