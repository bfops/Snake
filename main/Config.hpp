#pragma once

#include "Bounds.hpp"

struct Config
{
	struct SnakeConfig
	{
		unsigned int startingLength, width;
		unsigned int startingSpeed;

		unsigned int speedupPeriod, speedupAmount;

		double growthRate;
		unsigned int growthCap;
	};

	bool survival, music;

	unsigned short FPS;

	unsigned int spawnPeriod, spawnSize;
	unsigned int sentinelSize;

	unsigned int wallThickness;
	Bounds worldBounds;

	unsigned int pointGainPeriod, pointGainAmount;

	SnakeConfig snake;

	static Config& Get();
};
