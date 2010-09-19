#include "Snake.hpp"

#include "Common.hpp"
#include "GameWorld.hpp"
#include "Logger.hpp"

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace boost;

namespace {
DEF_CONSTANT(unsigned int, defaultLength, 90)
DEF_CONSTANT(unsigned int, snakeWidth, 20)
DEF_CONSTANT(unsigned int, speedupPeriod, 16000)
DEF_CONSTANT(unsigned int, speedupAmount, 23)
DEF_CONSTANT(unsigned int, growthCap, 100)
DEF_CONSTANT(double, linearGrowthRate, 10.0 / 29.0)
}

Snake::Snake(GameWorld& world) :
	logger(Logger::RequestHandle("Snake"))
{
	Reset(world);
}

namespace {
void add_segment(Snake::Path& path, Point location, Direction direction, GameWorld& world)
{
	SnakeSegment newSegment(location, direction, snakeWidth());

	for_each(path.begin(), path.end(), bind(&GameWorld::Delete, &world, _1));

	path.push_front(newSegment);

	for_each(path.begin(), path.end(), bind(&GameWorld::Add, &world, _1));
}
}

void Snake::Grow(int amount)
{
	if((int)projectedLength + amount < 1)
		projectedLength = 1;
	else
		projectedLength += amount;
}

inline SnakeSegment& Snake::Head()
{
	return path.front();
}
inline SnakeSegment& Snake::Tail()
{
	return path.back();
}

Direction get_random_direction()
{
	const static Direction directions[] = {Direction::left(), Direction::right(), Direction::up(), Direction::down()};
	uint32_t randomNumber = minstd_rand(time(NULL))();
	return directions[randomNumber % countof(directions)];
}
void Snake::Reset(GameWorld& world)
{
	Point headLocation = world.GetCenter();

	moveTimer.Reset();
	growTimer.Reset();
	speedupTimer.Reset();

	speed = 100;

	length = 0;
	projectedLength = defaultLength();
	path.clear();

	add_segment(path, headLocation, get_random_direction(), world);
}
void Snake::ChangeDirection(Direction newDirection, GameWorld& world)
{
	// TODO: change so that the new segment takes on the
	// "intersection" block between the new and old segment

	// TODO: if players enter two directions quickly enough,
	// they can u-turn into themselves. Fix.

	Direction direction(Head().GetDirection());

	if(newDirection != direction && newDirection != -direction)
	{
		// the point to start is the _direction_
		// side of the current head
		Bounds head = Head().GetHeadSquare();
		Side startSide = head.GetSide(newDirection);

		add_segment(path, startSide.min, newDirection, world);
	}
}

void Snake::Update(GameWorld& world)
{
	for(Path::iterator i = path.begin(), end = path.end(); i != end; ++i)
	{
		if(i->GetDigestionInfo() != SnakeSegment::HUNGRY)
		{
			double foodConstant = i->GetDigestionInfo();
			double baseUncappedGrowth = projectedLength * linearGrowthRate();
			double baseRealGrowth = min((double)growthCap(), baseUncappedGrowth);

			int growthAmount = round(baseRealGrowth * foodConstant);
			logger.Debug(format("Growing by %1%") % growthAmount);
			Grow(growthAmount);

			i->Digest();
		}
	}

	while(speedupTimer.ResetIfHasElapsed(speedupPeriod()))
		speed += speedupAmount();

	while(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		++Head();

		if(length > projectedLength)
		{
			--Tail();
			--length;
		}

		if(length < projectedLength)
		{
			++length;
		}
		else
		{
			--Tail();

			if(Tail().IsEmpty())
			{
				world.Delete(Tail());
				path.pop_back();
			}
		}
	}
}

bool Snake::IsDead() const
{
	for(Path::const_iterator i = path.begin(), end = path.end(); i != end; ++i)
		if(i->IsDead())
			return true;

	return false;
}
