#pragma once

#include "Clock.hpp"
#include "Food.hpp"
#include "Mine.hpp"
#include "Mutex.hpp"
#include "Snake.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Wall.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/thread.hpp>
#include <list>
#include <memory>
#include <SDL_events.h>

#ifdef MSVC
#pragma warning(pop)
#endif

struct ZippedUniqueObjectCollection;

class GameWorld
{
public:
	typedef std::auto_ptr<Spawn> SpawnPtr;
	typedef std::list<SpawnPtr> SpawnCollection;
	// a spawn list whose key is expiry time
	typedef std::map<Clock::TimeType, SpawnCollection> FunctionalSpawnCollection;
	typedef std::vector<Wall> WallCollection;

private:
	void SpawnLoop();

	ZippedUniqueObjectCollection& gameObjects;

	FunctionalSpawnCollection spawns;
	Mutex spawnMutex;

	boost::thread spawnThread;

	// whether or not to reset the game
	bool reset;

	Snake player;

	WallCollection walls;

	void Init();

public:
	GameWorld(ZippedUniqueObjectCollection& gameObjects);

	void Update();
	void Reset();

	// handle all the non object-specific side-effects of collisions (e.g. sound effects)
	void CollisionHandler(WorldObject&, WorldObject&);

	void KeyNotify(SDLKey key);
	void MouseNotify(Uint8 mouseButton);
};
