#include <ctime>
#include <cstdio>

#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Screen.hpp"
#include "Snake.hpp"
#include "Physics.hpp"

using namespace boost;
using namespace std;

static bool quit_called();

// TODO: fetch this dynamically
const unsigned int FPS = 60;

int main()
{
	// initialize random seed
	srand(time(NULL));

	Screen screen(810, 600, 54, 40);
	Snake player;

	// TODO: abstract this block out
	Wall walls[] = {
		Wall(Point(0, 0), 1, screen.bottomRight.y),
		Wall(Point(screen.bottomRight.x - 1, 0), 1, screen.bottomRight.y),
		Wall(Point(0, 0), screen.bottomRight.x, 1),
		Wall(Point(0, screen.bottomRight.y - 1), screen.bottomRight.x, 1)
	};
	for(int i = countof(walls) - 1; i >= 0; --i)
	{
		PhysicsWorld::AddObject(walls[i]);
	}

	player.SetRenderTarget(screen);
	player.Center();

	SDL_ShowCursor(SDL_DISABLE);

	// TODO: use more interrupts rather than loops
	// game loop
	while(!quit_called())
	{
		player.Update();
		PhysicsWorld::Update();

		screen.Clear();
		for(int i = countof(walls); i >= 0; --i)
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

bool quit_called()
{
	SDL_Event event;
	return (SDL_PollEvent(&event) != 0 && event.type == SDL_QUIT);
}

