#pragma once

#include "Bounds.hpp"
#include "Color24.hpp"
#include "Screen.hpp"

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
	ObjectType type;
	Bounds bounds;
	Color24 color;

<<<<<<< HEAD
public:
	WorldObject(ObjectType);
=======
		WorldObject& AddToWorld();
		WorldObject& RemoveFromWorld();
>>>>>>> 4d48b061e698573c41522b13249ac0ce91905ee2

	ObjectType GetObjectType() const;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
	void Draw(Screen& target) const;

	Bounds GetBounds() const;
	Color24 GetColor() const;
};
