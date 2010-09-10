#pragma once

#include "Screen.hpp"

namespace World
{
	// this is only temporary; later the render target
	// should be stored internally, maybe modify through functions
	void Update(Screen& renderTarget);
}
