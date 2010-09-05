#ifndef __Wall__
#define __Wall__

#include "Color24.hpp"
#include "Point.hpp"
#include "WorldObject.hpp"

class Wall : public WorldObject
{
public:
	Color24 color;

	Wall();
	Wall(Point location, size_t width, size_t height);

	ObjectType GetObjectType() const;
	void CollisionHandler(const WorldObject&);
};

#endif