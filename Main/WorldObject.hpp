#pragma once

#include "Color24.hpp"
#include "Point.hpp"
#include "Screen.hpp"

namespace World
{
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
		bool inWorld;
		ObjectType type;

		Point minBounds;
		Point maxBounds;

		Color24 color;

	public:
		WorldObject(ObjectType);
		WorldObject(const WorldObject&);
		WorldObject& operator=(const WorldObject&);
		virtual ~WorldObject();

		void AddToWorld();
		void RemoveFromWorld();

		ObjectType GetObjectType() const;
		virtual void CollisionHandler(const WorldObject& colidee) = 0;
		void Draw(Screen& target) const;

		Point GetMinBounds() const;
		Point GetMaxBounds() const;
		Color24 GetColor() const;
	};
}
