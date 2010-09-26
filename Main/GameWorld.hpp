#pragma once

#include "Food.hpp"
#include "SentinelFood.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "Wall.hpp"

#include <boost/array.hpp>
#include "SDL/SDL_events.h"
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

	void Init();

public:
	GameWorld(ZippedUniqueObjectList& gameObjects);

	void Update(ZippedUniqueObjectList& gameObjects, unsigned int elapsedMilliseconds);
	void Reset(ZippedUniqueObjectList& gameObjects);

	void KeyNotify(SDLKey key, ZippedUniqueObjectList& gameObjects);
	void MouseNotify(Uint8 button, ZippedUniqueObjectList& gameObjects);

	bool Lost() const;

	Point GetCenter() const;
};
