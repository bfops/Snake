#pragma once

#include "Food.hpp"
#include "SentinelFood.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "Wall.hpp"

#include <boost/array.hpp>
#include "SDL/SDL_events.h"
#include <vector>

class UniqueObjectList;

class GameWorld
{
public:
	typedef std::vector<Food> Menu;
	typedef std::vector<SentinelFood> SentinelList;
	typedef boost::array<Wall, 4> WallBox;

private:
	SentinelList sentinels;

	bool quit;
	Timer foodTimer;
	Menu foods;
	Snake player;

	WallBox walls;

	void Init();

public:
	GameWorld(UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);

	void Update(UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);
	void Reset(UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);

	void QuitNotify();
	void KeyNotify(SDLKey sym, UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);

	bool Lost() const;
	bool QuitCalled() const;

	Point GetCenter() const;
};
