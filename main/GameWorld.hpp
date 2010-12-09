#pragma once

#include "Common.hpp"
#include "Mine.hpp"
#include "Food.hpp"
#include "Sentinel.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "Wall.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <list>
#include <SDL_events.h>

#ifdef MSVC
#pragma warning(pop)
#endif

// random game-changing things //

// whether or not extra walls are added for epicness
//#define COOLERWALLS

struct ZippedUniqueObjectList;

class GameWorld
{
public:
	typedef std::list<Mine> MineList;
	typedef std::list<Food> Menu;
	typedef std::list<Sentinel> SentinelList;
	typedef std::vector<Wall> WallBox;

private:
	void FoodLoop();
	void MineLoop();

	ZippedUniqueObjectList& gameObjects;

	Menu foods;
	boost::mutex foodMutex;

	boost::thread spawnThread;
	bool reset;

	Snake player;

	WallBox walls;

	void Init(bool survivalMode);

public:
	GameWorld(ZippedUniqueObjectList& gameObjects);

	void Update();
	void Reset();

	/// this function handles all the World-level
	/// side-effects of collisions (e.g. sound effects)
	void CollisionHandler(WorldObject&, WorldObject&);
	void KeyNotify(SDLKey key);
	void MouseNotify(Uint8 button);

	Point GetCenter() const;
};
