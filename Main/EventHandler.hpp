#pragma once

class GameWorld;
class UniqueObjectList;

class EventHandler
{
public:
	void Update(GameWorld& world, UniqueObjectList& gameObjects);
};
