#include "Sentinel.hpp"

Sentinel::Sentinel(const Point location, const Config::SpawnsData::SpawnData& _spawnData) :
	WorldObject(sentinel), spawnData(&_spawnData)
{
	bounds.min = bounds.max = location;
	bounds.max.x += spawnData->size + spawnData->cushion;
	bounds.max.y += spawnData->size + spawnData->cushion;
}

void Sentinel::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

const Config::SpawnsData::SpawnData& Sentinel::GetSpawnData() const
{
	return *spawnData;
}
