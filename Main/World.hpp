#pragma once

#include "Screen.hpp"

namespace World
{
	// _renderTarget_ is only temporary; later it
	// should be stored internally
	void Update(Screen& renderTarget);
	void Reset();
}
