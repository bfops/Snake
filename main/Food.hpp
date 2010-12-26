#pragma once

#include "Spawn.hpp"

class Food : public Spawn
{
private:
	// effects on snake
	long long pointChange;
	double lengthFactor;
	short speedChange;

public:
	Food(Point location, unsigned short size, Color24, long long pointChange, double lengthFactor,
		short speedChange);

	void CollisionHandler(WorldObject&) const;

	long long GetPointChange() const;
	double GetLengthFactor() const;
	short GetSpeedChange() const;
};
