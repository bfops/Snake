#pragma once

#include "Common.hpp"
#include "Counter.hpp"
#ifdef SURVIVAL
#include "Mine.hpp"
#else
#include "Food.hpp"
#endif
#include "Sentinel.hpp"
#include "Snake.hpp"
#include "Wall.hpp"

#include <boost/array.hpp>
#include <SDL_events.h>
#include <vector>

struct ZippedUniqueObjectList;

class GameWorld
{
public:
#ifdef SURVIVAL
	typedef std::vector<Mine> MineList;
#else
	typedef std::vector<Food> Menu;
#endif
	typedef std::vector<Sentinel> SentinelList;
#ifdef COOLERWALLS
	typedef boost::array<Wall, 8> WallBox;
#else
	typedef boost::array<Wall, 4> WallBox;
#endif

private:
	SentinelList sentinels;

#ifdef SURVIVAL
	Counter mineTimer;
	MineList mines;
#else
	Counter foodTimer;
	Menu foods;
#endif
	Snake player;

	WallBox walls;

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

	Point GetCenter() const;
};
