#include "Screen.hpp"
#include "Snake.hpp"

#include <cassert>

using namespace std;

Snake::Snake()
{
	Reset();
}

void Snake::Reset()
{
	length = 1;
	path = vector<Direction>();
}
void Snake::GetInput()
{
	assert(!"Snake::GetInput");
}
void Snake::Update()
{
	assert(!"Snake::Update");
}
void Snake::Draw(Screen& target) const
{
	assert(!"Snake::Draw");
}
bool Snake::IsDead() const
{
	assert(!"Snake::Dead");
	return false;
}
