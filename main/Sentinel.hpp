#pragma once

#include "Config.hpp"
#include "WorldObject.hpp"

// Sentinels are sent out before spawns to check for collisions
// and thus clear a small (square) area for spawns
class Sentinel : public WorldObject
{
private:
	const Config::SpawnsData::SpawnData* spawnData;

public:
	Sentinel(Point location, const Config::SpawnsData::SpawnData& spawnData);
	
	void CollisionHandler(WorldObject&) const;

	const Config::SpawnsData::SpawnData& GetSpawnData() const;
};
