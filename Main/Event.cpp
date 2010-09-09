#include "Common.hpp"
#include "Event.hpp"
#include "Snake.hpp"

namespace Event {

namespace {
Snake* player;
bool* quit;

struct Initializer
{
	Initializer()
	{
		player = NULL;
		quit = NULL;
	}
} init;
}

static int key_handler(SDLKey sym)
{
	assert(player);

	switch(sym)
	{
		case SDLK_UP:
			player->ChangeDirection(Snake::up);
			break;
		case SDLK_DOWN:
			player->ChangeDirection(Snake::down);
			break;
		case SDLK_LEFT:
			player->ChangeDirection(Snake::left);
			break;
		case SDLK_RIGHT:
			player->ChangeDirection(Snake::right);
			break;

		default:
			return 1;
	}
	return 0;
}
static int quit_handler()
{
	assert(quit);

	*quit = true;
	return 0;
}

int SDLCALL Handler(const SDL_Event* event)
{
	switch(event->type)
	{
		case SDL_KEYDOWN:
			return key_handler(event->key.keysym.sym);

		case SDL_QUIT:
			return quit_handler();

		default:
			return 1;
	}
	return 0;
}
void RegisterPlayer(Snake& _player)
{
	player = &_player;
}
void RegisterQuitter(bool& quitter)
{
	quit = &quitter;
}
}
