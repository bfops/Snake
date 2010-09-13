#pragma once

#include "Bounds.hpp"
#include "Point.hpp"
#include "Screen.hpp"

namespace World
{
	void Update();
	void Reset();
	Bounds GetBounds();
	Point GetCenter();
}
