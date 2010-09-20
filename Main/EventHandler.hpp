#pragma once

#include "UniqueObjectList.hpp"

class GameWorld;
class EventHandler
{
public:
	void Update(GameWorld& world, UniqueObjectList& gameObjects);
};
