#pragma once

#include "Mine.hpp"
#include "Food.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "Wall.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/thread.hpp>
#include <list>
#include <SDL_events.h>

#ifdef MSVC
#pragma warning(pop)
#endif

struct ZippedUniqueObjectList;

class GameWorld
{
public:
	typedef std::list<Mine> MineList;
	typedef std::list<Food> Menu;
	typedef std::vector<Wall> WallList;

private:
	void FoodLoop();
	void MineLoop();

	ZippedUniqueObjectList& gameObjects;

	Menu foods;
	boost::mutex foodMutex;

	boost::thread spawnThread;

	// whether or not to reset the game
	bool reset;

	Snake player;

	WallList walls;

	void Init();

public:
	GameWorld(ZippedUniqueObjectList& gameObjects);

	void Update();
	void Reset();

	// handle all the non object-specific side-effects of collisions (e.g. sound effects)
	void CollisionHandler(WorldObject&, WorldObject&);

	void KeyNotify(SDLKey key);
	void MouseNotify(Uint8 mouseButton);
};
