#include "Config.hpp"

std::stringstream Config::defaultConfig(std::string("\
survival 0\
music 0\
sound 0\
FPS 60\
\
{ screen\
	w 800\
	h 600\
	{ color r 0 g 0 b 0 }\
}\
\
{ resources\
	eat resources/eat.wav\
	spawn resources/spawn.wav\
	die resources/death.wav\
	theme resources/theme.wav\
}\
\
{ worldBounds\
	{ min x 0   y 0   }\
	{ max x 800 y 600 }\
}\
\
{ walls\
	{ color r 255 g 0 b 0 }\
	{ wall { min x 0   y 0   } { max x 10  y 600 } }\
	{ wall { min x 790 y 0   } { max x 800 y 600 } }\
	{ wall { min x 10  y 0   } { max x 790 y 10  } }\
	{ wall { min x 10  y 590 } { max x 790 y 600 } }\
	{ wall { min x 10  y 295 } { max x 200 y 305 } }\
	{ wall { min x 600 y 295 } { max x 790 y 305 } }\
	{ wall { min x 395 y 10  } { max x 405 y 150 } }\
	{ wall { min x 395 y 400 } { max x 405 y 590 } }\
}\
\
{ spawn\
	{ mineColor r 255 g 0 b 255 }\
	{ foods\
		{ food rate 0.05 lengthFactor -2.5 points -300 { color r 0   g 0   b 255 } }\
		{ food rate 0.1  lengthFactor 0.3  points 25   { color r 127 g 255 b 127 } }\
		{ food rate 0.2  lengthFactor 1    points 100  { color r 0   g 255 b 255 } }\
		{ food rate 0.65 lengthFactor 3    points 400  { color r 200 g 0   b 0   } }\
	}\
}\
\
{ survival\
	pointGainPeriod 10000\
	pointGainAmount 150\
	{ snake speedupPeriod 20000 }\
	{ spawn \
		additionPeriod 3000\
		size 10\
		sentinelSize 20\
	}\
}\
\
{ normal\
	pointGainPeriod 5000\
	pointGainAmount 15\
	{ snake speedupPeriod 14000 }\
	{ spawn\
		additionPeriod 8000\
		size 15\
		sentinelSize 17\
	}\
}\
\
{ snake\
	defaultLength 90\
	width 20\
	defaultSpeed 100\
	speedupAmount 18\
	growthCap 100\
	growthRate 0.345\
	{ color r 0 g 255 b 0 }\
}\
"));