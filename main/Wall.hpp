#pragma once

#include "Config.hpp"
#include "WorldObject.hpp"

class Wall : public WorldObject
{
public:
	Wall(const Config::WallData& wallData);

	void CollisionHandler(WorldObject&) const;
};
