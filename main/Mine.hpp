#pragma once

#include "Config.hpp"
#include "Spawn.hpp"

// a snake-killing mine
class Mine : public Spawn
{
public:
	Mine(Point location, const Config::SpawnsData::MineData& mineData);

	void CollisionHandler(WorldObject&) const;
};
