#pragma once

#include "Screen.hpp"
#include "WorldObject.hpp"

namespace GraphicsWorld
{
	void Add(const WorldObject&);
	void Remove(const WorldObject&);
	void Update(Screen& target);
}