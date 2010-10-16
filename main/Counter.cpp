#include "Counter.hpp"

Counter::Counter()
{
	Reset();
}

void Counter::Reset()
{
	elapsedCount = 0;
}

void Counter::Update(unsigned int count)
{
	elapsedCount += count;
}

bool Counter::ResetIfHasElapsed(unsigned int c)
{
	if(elapsedCount >= c)
	{
		elapsedCount -= c;
		return true;
	}

	return false;
}
