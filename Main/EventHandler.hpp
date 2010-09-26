#pragma once

class GameState;
class ZippedUniqueObjectList;

class EventHandler
{
public:
	void Update(GameState& gameState, ZippedUniqueObjectList& gameObjects);
};
