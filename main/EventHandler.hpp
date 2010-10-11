#pragma once

class GameState;
class ZippedUniqueObjectList;

namespace EventHandler
{
	void HandleEventQueue(GameState& gameState, ZippedUniqueObjectList& gameObjects);
}
