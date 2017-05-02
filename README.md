This is a combined effort (mainly mine) at making a cross-platform version of Snake! This is my first attempt at a game, so I'm still riding the learning curve quite a bit.


# THE GAME

So far, it's really not much. There is game configuration in game.cfg (see CONFIG).

The game is played with a constantly-moving Snake. The Snake is turned using the arrow keys or mouse, but cannot be stopped. Turning the Snake so that it collides with a wall or itself causes the player to lose. The object of the game is to get as many points as possible before dying. Points can be gained or lost by eating different types of food. The Snake also grows longer after eating food, and gains points or speed intermittently.

Additionally, there are magenta mines which appear, which cause the player to die upon contact


## LANGUAGE

This is written in C++ with Boost & SDL, with the nonstandard addition of #pragma once, due to common usage. As far as I'm aware, everything else is standards-conformant and should compile & run on any platform.


## DEPENDENCIES

To compile, you will need:
- Boost C++ libraries (http://www.boost.org/users/download/)
- SDL SDK (http://www.libsdl.org/)
- SDL Runtime (http://www.libsdl.org/)
- CMake (http://www.cmake.org/cmake/resources/software.html)

To work with the audio files, you'll need a MIDI editor (I'm using Finale 2011)


## SNAKE GROWTH

Snake growth is based on multiple factors:
- Linear coefficient of food
- Global linear coefficient
- Current snake length
- Growth cap
	
The amount by which the snake grows is:
	min(length * global coefficient, growthCap) * food coefficient
	
It should be noted that the growth cap caps growth BEFORE being multiplied by the food coefficient.


## CONFIG

The game config file (game.cfg) is organized as sets of key-value pairs. Related keys can be organized in Scopes.

Scopes are collections of key-value pairs surrounded by curly braces. The first string inside the curly braces is the Scope's name. Scopes can be nested within one another.

### music/sound: Toggle audio effects (1 or 0)
FPS: The approximate FPS at which the game should run (unsigned short)

### screen:
	w/h: Width/height (unsigned long)
	color: background color (3 unsigned bytes)
	
### resources:
	eat: path of the eating sound
	spawn: path of the spawning sound
	die: path of the dying sound
	theme: path of the theme music
	
### worldBounds: boundaries of the world (dictates the regions in which spawns can spawn)
	min/max: pairs of (long, long)
	
### walls: the set of all wall data
	color: color of all the walls
	wall:
		min/max: rectangular bounds of the wall, as (long, long) pairs
		
### spawns: all the spawn data (foods, mines)
1. period: the interval of time between spawn appearances (unsigned int)
2. mines: collection of mine data
		mine: snake-killing mines
			size: square size of the mine (unsigned short)
			cushion: amount of space required around the mine for it to spawn (unsigned short)
			expiry: time it takes in milliseconds for the mine to disappear (unsigned int)
3. foods: collection of food data
		food:
			rate: the odds that every spawn period, this food appears (double)
			lengthFactor: a linear coefficient of the snake's growth amount (double)
			points: points given for eating this food (long long)
			speedChange: amount by which the food changes the snake speed (short)
		
### pointGainPeriod: the interval of time between snake point gains/losses
### pointGainAmount: how many points the player gains each pointGainPeriod (long long)
		
### snake:
1. **startingLength:** the starting length of the snake (unsigned long)
2. **width:** the width of the snake (unsigned short)
3. **startingSpeed:** the initial frequency at which the snake moves (unsigned short)
4. **speedupAmount:** the amount by which snake speed  changes, every speedupPeriod (unsigned short)
5. **speedupPeriod:** the interval of time between snake speedups (unsigned int)
6. **growthCap:** the highest amount by which the snake can grow at once, before being multiplied by the food's growth coefficient (unsigned long)
7. **growthRate:** a linear coefficient of the snake's growth amount (double)
