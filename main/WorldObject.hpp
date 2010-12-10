#pragma once

#include "Bounds.hpp"
#include "Color24.hpp"

class Screen;
class Food;
class Mine;
class Sentinel;
class SnakeSegment;
class Wall;

class WorldObject
{
public:
	enum ObjectType
	{
		snake = 1,
		wall = 1<<1,
		food = 1<<2,
		sentinel = 1<<3,
		mine = 1<<4
	};

protected:
	ObjectType type;
	Bounds bounds;
	Color24 color;

public:
	WorldObject(ObjectType);
	WorldObject(ObjectType, const Color24 color);
	
	virtual void CollisionHandler(WorldObject&) const;
	virtual void CollisionHandler(const Food&);
	virtual void CollisionHandler(const Mine&);
	virtual void CollisionHandler(const Sentinel&);
	virtual void CollisionHandler(const SnakeSegment&);
	virtual void CollisionHandler(const Wall&);

	ObjectType GetObjectType() const;
	void Draw(Screen& target) const;

	Bounds GetBounds() const;
	Color24 GetColor() const;
};
