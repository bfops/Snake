#include "Spawn.hpp"

#include "Config.hpp"
#include "Logger.hpp"

Spawn::Spawn(const ObjectType spawnType, const Point loc, const Config::SpawnsData::SpawnData& _spawnData) :
	WorldObject(spawnType, _spawnData.color), spawnData(&_spawnData)
{
	const unsigned short cushionedSize = spawnData->size + spawnData->cushion;
	bounds.min = bounds.max = loc;
	bounds.max.x += cushionedSize;
	bounds.max.y += cushionedSize;
}

Spawn::~Spawn()
{
}

void Spawn::ShrinkDown()
{
	bounds.min.x += spawnData->cushion / 2;
	bounds.min.y += spawnData->cushion / 2;
	bounds.max = bounds.min;
	bounds.max.x = bounds.min.x + spawnData->size;
	bounds.max.y = bounds.min.y + spawnData->size;
}

const Config::SpawnsData::SpawnData& Spawn::GetSpawnData() const
{
	return *spawnData;
}
