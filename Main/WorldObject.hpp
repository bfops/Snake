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
		wall = 2,
		food = 2<<1,
		sentinelFood = 2<<2
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
