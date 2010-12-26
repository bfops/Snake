#include "Config.hpp"

std::stringstream Config::GetDefaultConfig()
{
	return std::stringstream(std::string("\n\
music 1\n\
sound 1\n\
FPS 60\n\
\n\
{ screen\n\
	w 800\n\
	h 600\n\
	{ color r 0 g 0 b 0 }\n\
}\n\
\n\
{ resources\n\
	eat resources/eat.wav\n\
	spawn resources/spawn.wav\n\
	die resources/death.wav\n\
	theme resources/theme.wav\n\
}\n\
\n\
{ walls\n\
	{ wall { bounds { min x 0   y 0   } { max x 10  y 600 } } { color r 255 g 0 b 0 } }\n\
	{ wall { bounds { min x 790 y 0   } { max x 800 y 600 } } { color r 255 g 0 b 0 } }\n\
	{ wall { bounds { min x 10  y 0   } { max x 790 y 10  } } { color r 255 g 0 b 0 } }\n\
	{ wall { bounds { min x 10  y 590 } { max x 790 y 600 } } { color r 255 g 0 b 0 } }\n\
	{ wall { bounds { min x 10  y 295 } { max x 200 y 305 } } { color r 255 g 0 b 0 } }\n\
	{ wall { bounds { min x 600 y 295 } { max x 790 y 305 } } { color r 255 g 0 b 0 } }\n\
	{ wall { bounds { min x 395 y 10  } { max x 405 y 150 } } { color r 255 g 0 b 0 } }\n\
	{ wall { bounds { min x 395 y 400 } { max x 405 y 590 } } { color r 255 g 0 b 0 } }\n\
}\n\
\n\
{ spawns\n\
	period 8000\n\
	{ bounds { min x 0 y 0 } { max x 800 y 600 } }\n\
	{ mines\n\
		{ mine\n\
			size 10 cushion 10 expiry 60000 rate 0.1\n\
			{ color r 255 g 0 b 255 }\n\
		}\n\
		{ mine\n\
			size 13 cushion 0 expiry 1000000 rate 0\n\
			{ color r 255 g 255 b 255 }\n\
		}\n\
	}\n\
	{ foods\n\
		{ food\n\
			size 15 cushion 0 expiry 20000 rate 0.05\n\
			lengthFactor -2.5 points -200 speedChange 10\n\
			{ color r 0 g 0 b 255 }\n\
		}\n\
		{ food\n\
			size 15 cushion 3 expiry 30000 rate 0.1\n\
			lengthFactor 0.3 points 25 speedChange 0\n\
			{ color r 127 g 255 b 127 }\n\
		}\n\
		{ food\n\
			size 15 cushion 2 expiry 40000 0.3\n\
			lengthFactor 1 points 100 speedChange 0\n\
			{ color r 0 g 255 b 255 }\n\
		}\n\
		{ food\n\
			size 14 cushion 0 expiry 150000 0.4\n\
			lengthFactor 3 points 400 speedChange 5\n\
			{ color r 200 g 0 b 0 }\n\
		}\n\
		{ food\n\
			size 10 cushion 10 expiry 15000 0.05\n\
			lengthFactor 0 points 0 speedChange -30\n\
			{ color r 255 g 255 b 0 }\n\
		}\n\
	}\n\
}\n\
\n\
pointGainPeriod 5000\n\
pointGainAmount 15\n\
\n\
{ snake\n\
	startingLength 90\n\
	width 20\n\
	startingSpeed 100\n\
	speedupAmount 15\n\
	speedupPeriod 14000\n\
	growthCap 100\n\
	growthRate 0.345\n\
	{ color r 0 g 255 b 0 }\n\
}\n\
"));
}
