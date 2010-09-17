#pragma once

#include "Food.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "World.hpp"

#include <vector>

class GameWorld
{
public:
	typedef std::vector<Food*> Menu;

private:
	Timer foodTimer;
	Menu foods;
	Snake player;

public:
	GameWorld(World& world);

	void Update(World& world);
	void Reset(World& world);

	bool Lost() const;
};
