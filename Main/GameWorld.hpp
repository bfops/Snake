#pragma once

#include "Food.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "UniqueObjectList.hpp"
#include "Wall.hpp"
#include "WorldObject.hpp"

#include <boost/array.hpp>
#include <SDL/SDL_events.h>
#include <vector>

class GameWorld
{
public:
	typedef std::vector<Food> Menu;
	typedef std::vector<SentinelFood> SentinelList;
	typedef boost::array<Wall, 4> WallBox;

private:
	SentinelList sentinels;

	Screen screen;

	bool quit;
	Timer foodTimer;
	Menu foods;
	Snake player;

	WallBox walls;

	void Init();

public:
	GameWorld(UniqueObjectList& gameObjects);

	void Update(UniqueObjectList& gameObjects);
	void Reset(UniqueObjectList& gameObjects);

	void QuitNotify();
	void KeyNotify(SDLKey sym, UniqueObjectList& gameObjects);

	bool Lost() const;
	bool QuitCalled() const;

	Point GetCenter() const;
};
