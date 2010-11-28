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
#ifdef SURVIVAL
	typedef std::list<Mine> MineList;
#else
	typedef std::list<Food> Menu;
#endif
	typedef std::list<Sentinel> SentinelList;
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

	/// this function handles all the World-level
	/// side-effects of collisions (e.g. sound effects)
	void CollisionHandler(ZippedUniqueObjectList& gameObjects, WorldObject&, WorldObject&);
	void KeyNotify(SDLKey key, ZippedUniqueObjectList& gameObjects);
	void MouseNotify(Uint8 button, ZippedUniqueObjectList& gameObjects);

	Point GetCenter() const;
};
