#pragma once

#include "WorldObject.hpp"

class Sentinel : public WorldObject
{
private:
	bool interfering;

	void Init();

public:
	Sentinel(Point location, unsigned int size);

	void CollisionHandler(const WorldObject&);

	bool IsInterfering() const;
};
