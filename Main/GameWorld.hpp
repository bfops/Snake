#pragma once

#include "Food.hpp"
#include "Timer.hpp"
#include "World.hpp"

#include <vector>

class GameWorld
{
private:
	Timer foodTimer;
	typedef std::vector<Food*> Menu;
	Menu foods;

public:
	GameWorld(World& world);

	void Update(World& world);
	void Reset(World& world);

	Bounds GetBounds() const;
};
