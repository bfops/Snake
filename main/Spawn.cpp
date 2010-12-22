#include "Spawn.hpp"

#include "Color24.hpp"
#include "Logger.hpp"
#include "Sentinel.hpp"

Spawn::Spawn(const ObjectType spawnType, const Sentinel& prototype, const unsigned short size,
	const Color24 color) :
	WorldObject(spawnType, color)
{
	// prototypes are guaranteed to be squares, so \Delta x = \Delta y
	const unsigned short prototypeSize = prototype.GetBounds().max.x - prototype.GetBounds().min.x;
	const short sizeDiff = prototypeSize - size;
	if(sizeDiff < 0)
	{
		Logger::Debug(boost::format("Spawn of type %1% was passed a sentinel smaller than the spawn size!")
			% spawnType);
		return;
	}

	bounds.min = prototype.GetBounds().min;
	bounds.min.x += sizeDiff / 2;
	bounds.min.y += sizeDiff / 2;
	bounds.max = bounds.min;
	bounds.max.x += size;
	bounds.max.y += size;
}

Spawn::~Spawn()
{
}
