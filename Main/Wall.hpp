#ifndef __Wall__
#define __Wall__

#include "WorldObject.hpp"

class Wall : public WorldObject
{
public:
	ObjectType GetObjectType() const;
};

#endif