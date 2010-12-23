#include "Sentinel.hpp"

Sentinel::Sentinel(const Point location, const Config::SpawnsData::SpawnData& _spawnData) :
	WorldObject(sentinel), spawnData(&_spawnData)
{
	const unsigned short sentinelSize = spawnData->size + spawnData->cushion;
	bounds.min = bounds.max = location;
	bounds.max.x += sentinelSize;
	bounds.max.y += sentinelSize;
}

void Sentinel::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

const Config::SpawnsData::SpawnData& Sentinel::GetSpawnData() const
{
	return *spawnData;
}
