#pragma once

// This class acts as a RAII keep-alive for SDL. As long as
// (1 and only 1) is constructed, SDL will stay usable.
struct SDLInitializer
{
public:
	SDLInitializer();
	~SDLInitializer();
};
