#pragma once

#include "Config.hpp"
#include "WorldObject.hpp"

struct Color24;

// spawnable objects should inherit from this (e.g. food, mines)
class Spawn : public WorldObject
{
protected:
	const Config::SpawnsData::SpawnData* spawnData;

public:
	// construct as cushioned size defined in _spawnData_
	Spawn::Spawn(ObjectType spawnType, Point location, const Config::SpawnsData::SpawnData& spawnData);
	virtual ~Spawn();

	// shrink from cushioned size to normal size
	void ShrinkDown();

	const Config::SpawnsData::SpawnData& GetSpawnData() const;
};
