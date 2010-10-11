#pragma once

class GameState;
class ZippedUniqueObjectList;

class EventHandler
{
public:
	/// returns true if an event was handled, false otherwise
	bool GetAndHandleEvent(GameState& gameState, ZippedUniqueObjectList& gameObjects);
};
