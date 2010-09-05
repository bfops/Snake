#include <ctime>
#include <cstdio>

#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "Physics.hpp"

using namespace boost;
using namespace std;

// TODO: fetch this dynamically
const unsigned int FPS = 60;

int main()
{
	// initialize random seed
	srand(time(NULL));
	SDL_SetEventFilter(Event::Handler);

	Screen screen(810, 600, 54, 40);

	bool quit = false;
	Snake player;

	Event::RegisterPlayer(player);
	Event::RegisterQuitter(quit);

	// TODO: abstract this block out
	Wall walls[] = {
		Wall(Point(0, 0), 1, screen.bottomRight.y),
		Wall(Point(screen.bottomRight.x - 1, 0), 1, screen.bottomRight.y),
		Wall(Point(0, 0), screen.bottomRight.x, 1),
		Wall(Point(0, screen.bottomRight.y - 1), screen.bottomRight.x, 1)
	};
	for(unsigned int i = 0; i < countof(walls); ++i)
	{
		PhysicsWorld::AddObject(walls[i]);
	}

	player.SetRenderTarget(screen);
	player.Center();

	SDL_ShowCursor(SDL_DISABLE);

	// TODO: use more interrupts rather than loops
	// game loop
	while(!quit)
	{
		player.Update();
		PhysicsWorld::Update();

		screen.Clear();
		for(unsigned int i = 0; i < countof(walls); ++i)
			screen.Draw(walls[i], walls[i].color);
		player.Draw();
		screen.Update();

		if(player.IsDead())
		{
			cout << "You lose!\n";
			break;
		}

		this_thread::sleep(posix_time::millisec(1000 / FPS));
	}

	return 0;
}
