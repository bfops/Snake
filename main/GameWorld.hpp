#pragma once

#include "Food.hpp"
#include "SentinelFood.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "Wall.hpp"

#include <boost/array.hpp>
#include <SDL_events.h>
#include <vector>

class ZippedUniqueObjectList;

class GameWorld
{
public:
	typedef std::vector<Food> Menu;
	typedef std::vector<SentinelFood> SentinelList;
	typedef boost::array<Wall, 4> WallBox;

private:
	SentinelList sentinels;

	Timer foodTimer;
	Menu foods;
	Snake player;

	WallBox walls;

	bool lost;

	void Init();

public:
	GameWorld(ZippedUniqueObjectList& gameObjects);

	void Update(ZippedUniqueObjectList& gameObjects, unsigned int elapsedMilliseconds);
	void Reset(ZippedUniqueObjectList& gameObjects);

	/// this function handles all the World-level side-effects of collisions
	/// (e.g. setting the "player died" variable, as well as sound effects)
	void CollisionNotify(WorldObject::ObjectType, WorldObject::ObjectType);
	void KeyNotify(SDLKey key, ZippedUniqueObjectList& gameObjects);
	void MouseNotify(Uint8 button, ZippedUniqueObjectList& gameObjects);

	bool Lost() const;

	Point GetCenter() const;
};
