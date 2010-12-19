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
{ spawn\n\
	{ mineColor r 255 g 0 b 255 }\n\
	{ foods\n\
		{ food rate 0.05 lengthFactor -2.5 points -300 { color r 0   g 0   b 255 } }\n\
		{ food rate 0.1  lengthFactor 0.3  points 25   { color r 127 g 255 b 127 } }\n\
		{ food rate 0.2  lengthFactor 1    points 100  { color r 0   g 255 b 255 } }\n\
		{ food rate 0.65 lengthFactor 3    points 400  { color r 200 g 0   b 0   } }\n\
	}\n\
}\n\
\n\
{ survival\n\
	pointGainPeriod 10000\n\
	pointGainAmount 150\n\
	{ snake speedupPeriod 20000 }\n\
	{ spawn \n\
		additionPeriod 3000\n\
		size 10\n\
		sentinelSize 20\n\
	}\n\
}\n\
\n\
{ normal\n\
	pointGainPeriod 5000\n\
	pointGainAmount 15\n\
	{ snake speedupPeriod 14000 }\n\
	{ spawn\n\
		additionPeriod 8000\n\
		size 15\n\
		sentinelSize 17\n\
	}\n\
}\n\
\n\
{ snake\n\
	defaultLength 90\n\
	width 20\n\
	defaultSpeed 100\n\
	speedupAmount 18\n\
	growthCap 100\n\
	growthRate 0.345\n\
	{ color r 0 g 255 b 0 }\n\
}\n\
"));
}