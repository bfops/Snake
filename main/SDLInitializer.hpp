#pragma once

#include "Logger.hpp"

// This class acts as a RAII keep-alive for SDL. As long as an (1 and only 1)
// instance is constructed, SDL will stay usable.
struct SDLInitializer
{
public:
	SDLInitializer();
	~SDLInitializer();
};
