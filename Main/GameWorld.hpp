#pragma once

#include "Food.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "ObjectManager.hpp"

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
	GameWorld(ObjectManager& world);

	void Update(ObjectManager& world);
	void Reset(ObjectManager& world);

	bool Lost() const;
};
