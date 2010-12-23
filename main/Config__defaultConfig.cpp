#include "Config.hpp"

std::stringstream Config::GetDefaultConfig()
{
	return std::stringstream(std::string("\n\
survival 0\n\
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
{ worldBounds\n\
	{ min x 0   y 0   }\n\
	{ max x 800 y 600 }\n\
}\n\
\n\
{ walls\n\
	{ color r 255 g 0 b 0 }\n\
	{ wall { min x 0   y 0   } { max x 10  y 600 } }\n\
	{ wall { min x 790 y 0   } { max x 800 y 600 } }\n\
	{ wall { min x 10  y 0   } { max x 790 y 10  } }\n\
	{ wall { min x 10  y 590 } { max x 790 y 600 } }\n\
	{ wall { min x 10  y 295 } { max x 200 y 305 } }\n\
	{ wall { min x 600 y 295 } { max x 790 y 305 } }\n\
	{ wall { min x 395 y 10  } { max x 405 y 150 } }\n\
	{ wall { min x 395 y 400 } { max x 405 y 590 } }\n\
}\n\
\n\
{ spawns\n\
	{ mine size 10 cushion 10 expiry 60000 { color r 255 g 0 b 255 } }\n\
	{ foods\n\
		{ food\n\
			size 15 cushion 0 expiry 20000\n\
			rate 0.05 lengthFactor -2.5 points -200 speedChange 10\n\
			{ color r 0 g 0 b 255 }\n\
		}\n\
		{ food\n\
			size 15 cushion 3 expiry 30000\n\
			rate 0.1 lengthFactor 0.3 points 25 speedChange 0\n\
			{ color r 127 g 255 b 127 }\n\
		}\n\
		{ food\n\
			size 15 cushion 2 expiry 40000\n\
			rate 0.35 lengthFactor 1 points 100 speedChange 0\n\
			{ color r 0 g 255 b 255 }\n\
		}\n\
		{ food\n\
			size 14 cushion 0 expiry 150000\n\
			rate 0.45 lengthFactor 3 points 400 speedChange 5\n\
			{ color r 200 g 0 b 0 }\n\
		}\n\
		{ food\n\
			size 10 cushion 10 expiry 15000\n\
			rate 0.05 lengthFactor 0 points 0 speedChange -30\n\
			{ color r 255 g 255 b 0 }\n\
		}\n\
	}\n\
}\n\
\n\
{ survival\n\
	pointGainPeriod 10000\n\
	pointGainAmount 150\n\
	{ snake speedupPeriod 20000 }\n\
	{ spawn period 3000 }\n\
}\n\
\n\
{ normal\n\
	pointGainPeriod 5000\n\
	pointGainAmount 15\n\
	{ snake speedupPeriod 14000 }\n\
	{ spawn period 8000 }\n\
}\n\
\n\
{ snake\n\
	startingLength 90\n\
	width 20\n\
	startingSpeed 100\n\
	speedupAmount 15\n\
	growthCap 100\n\
	growthRate 0.345\n\
	{ color r 0 g 255 b 0 }\n\
}\n\
"));
}
