#pragma once

#include "Common.hpp"
#include "Counter.hpp"
#include "Mine.hpp"
#include "Food.hpp"
#include "Sentinel.hpp"
#include "Snake.hpp"
#include "Wall.hpp"

#ifdef MSVC
#pragma warning( push, 0 )
#endif

#include <boost/array.hpp>
#include <list>
#include <SDL_events.h>

#ifdef MSVC
#pragma warning( pop )
#endif

// random game-changing things //

// whether or not extra walls are added for epicness
#define COOLERWALLS
// whether or not survival mode is on
//#define SURVIVAL

struct ZippedUniqueObjectList;

class GameWorld
{
public:
	typedef std::list<Mine> MineList;
	typedef std::list<Food> Menu;
	typedef std::list<Sentinel> SentinelList;
#ifdef COOLERWALLS
	typedef boost::array<Wall, 8> WallBox;
#else
	typedef boost::array<Wall, 4> WallBox;
#endif

private:
	SentinelList sentinels;

	Counter mineTimer;
	MineList mines;
	Counter foodTimer;
	Menu foods;
	Snake player;

	WallBox walls;

	void Init();

public:
	GameWorld(ZippedUniqueObjectList& gameObjects);

	void Update(ZippedUniqueObjectList& gameObjects, unsigned int elapsedMilliseconds);
	void Reset(ZippedUniqueObjectList& gameObjects);

	/// this function handles all the World-level
	/// side-effects of collisions (e.g. sound effects)
	void CollisionHandler(WorldObject&, WorldObject&);
	void KeyNotify(SDLKey key, ZippedUniqueObjectList& gameObjects);
	void MouseNotify(Uint8 button, ZippedUniqueObjectList& gameObjects);

	Point GetCenter() const;
};
