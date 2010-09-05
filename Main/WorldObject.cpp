#include <cassert>

#include "WorldObject.hpp"

WorldObject::ObjectType WorldObject::GetObjectType() const
{
	assert(!"No override specified! Panicpanicpanic");
	return generic;
}
