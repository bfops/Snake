#pragma once

#include "WorldObject.hpp"

class SentinelFood : public WorldObject
{
private:
	bool interfering;

public:
	SentinelFood();
	SentinelFood(Point location, unsigned int size);

	void CollisionHandler(const WorldObject&);

	bool IsInterfering() const;
};
