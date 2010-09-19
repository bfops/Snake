#pragma once

#include "Food.hpp"
#include "Logger.hpp"
#include "Snake.hpp"
#include "Timer.hpp"
#include "Wall.hpp"
#include "WorldObject.hpp"

#include <boost/array.hpp>
#include <SDL/SDL_events.h>
#include <vector>

class GameWorld
{
public:
	class EventHandler
	{
	private:
		GameWorld* const world;

	public:
		EventHandler(GameWorld& parent);

		void Update();
	};

	typedef std::vector<WorldObject*> ObjectList;
	typedef std::vector<Food> Menu;
	typedef boost::array<Wall, 4> WallBox;

private:
	Logger::Handle logger;

	SentinelFood sentinel;
	bool sentinelSent;

	Screen screen;
	ObjectList objects;

	bool quit;
	Timer foodTimer;
	Menu foods;
	Snake player;

	EventHandler eventHandler;

	WallBox walls;

public:
	GameWorld();

	void Update();
	void Reset();

	void QuitNotify();
	void KeyNotify(SDLKey sym);

	bool Lost() const;
	bool QuitCalled() const;

	Point GetCenter() const;

	void Add(WorldObject&);
	void Delete(WorldObject&);
};
