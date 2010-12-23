#include "Spawn.hpp"

#include "Config.hpp"
#include "Logger.hpp"
#include "Sentinel.hpp"

Spawn::Spawn(const ObjectType spawnType, const Sentinel& prototype) :
	WorldObject(spawnType, prototype.GetSpawnData().color), spawnData(&prototype.GetSpawnData())
{
	bounds.min = prototype.GetBounds().min;
	bounds.min.x += spawnData->cushion;
	bounds.min.y += spawnData->cushion;
	bounds.max = bounds.min;
	bounds.max.x += spawnData->size;
	bounds.max.y += spawnData->size;
}

Spawn::~Spawn()
{
}

const Config::SpawnsData::SpawnData& Spawn::GetSpawnData() const
{
	return *spawnData;
}
