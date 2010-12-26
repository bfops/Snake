#include "Spawn.hpp"

#include "Common.hpp"
#include "Logger.hpp"

Spawn::Spawn(const ObjectType spawnType, const Point loc, const unsigned short size, const Color24 color) :
	WorldObject(spawnType, color)
{
	bounds.min = bounds.max = loc;
	bounds.max.x += size;
	bounds.max.y += size;
}

Spawn::~Spawn()
{
}

void Spawn::ShrinkDown(const unsigned short newSize)
{
	DOLOCKED(mutex,
		const unsigned short size = bounds.max.x - bounds.min.x;
		if(newSize > size)
			Logger::Debug("New size passed to Spawn::ShrinkDown is greater than current size");

		const unsigned short sizeDiff = size - newSize;
		bounds.min.x += sizeDiff / 2;
		bounds.min.y += sizeDiff / 2;
		bounds.max.x = bounds.min.x + newSize;
		bounds.max.y = bounds.min.y + newSize;
	)
}
