#pragma once

#include <cstdlib>

#include "Color24.hpp"
#include "Point.hpp"
#include "Screen.hpp"

// TODO: make this be a generic object for
// both Physics and Graphics worlds
class WorldObject
{
public:
	enum ObjectType
	{
		snake,
		wall,
		food
	};

protected:
	bool inPhysics;
	ObjectType type;

public:
	Point location;
	unsigned int width, height;
	Color24 color;

	WorldObject(ObjectType);
	WorldObject(const WorldObject&);
	virtual ~WorldObject();

	virtual void AddToWorld();
	virtual void RemoveFromWorld();

	virtual ObjectType GetObjectType() const;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
	virtual void Draw(Screen& target) const;
};
