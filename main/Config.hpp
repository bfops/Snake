#pragma once

#include "Bounds.hpp"

struct Config
{
	struct SnakeConfig
	{
		unsigned int startingLength;
		unsigned int width;
		unsigned int startingSpeed;

		unsigned int speedupPeriod;
		unsigned int speedupAmount;

		double growthRate;
		unsigned int growthCap;
	};

	bool survival;
	unsigned int spawnPeriod;
	unsigned int spawnSize;
	unsigned int sentinelSize;

	unsigned int wallThickness;
	Bounds worldBounds;

	unsigned int pointGainPeriod;
	unsigned int pointGainAmount;

	SnakeConfig snake;

	static Config& Get();
};
