#pragma once

#include "Bounds.hpp"
#include "Color24.hpp"

class Screen;

class WorldObject
{
public:
	enum ObjectType
	{
		snake = 1,
		wall = 1<<1,
		food = 1<<2,
		sentinelFood = 1<<3,
		mine = 1<<4
	};

protected:
	ObjectType type;
	Bounds bounds;
	Color24 color;

public:
	WorldObject(ObjectType);

	ObjectType GetObjectType() const;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
	void Draw(Screen& target) const;

	Bounds GetBounds() const;
	Color24 GetColor() const;
};
