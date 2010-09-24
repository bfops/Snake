#pragma once

class GameWorld;
class ZippedUniqueObjectList;

class EventHandler
{
public:
	void Update(GameWorld& world, ZippedUniqueObjectList& gameObjects);
};
