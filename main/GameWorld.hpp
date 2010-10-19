#pragma once

#include "Common.hpp"
#include "Counter.hpp"
#include "Food.hpp"
#include "SentinelFood.hpp"
#include "Snake.hpp"
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
#ifdef COOLERWALLS
	typedef boost::array<Wall, 8> WallBox;
#else
	typedef boost::array<Wall, 4> WallBox;
#endif

private:
	SentinelList sentinels;

	Counter foodTimer;
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
