#include "Snake.hpp"

#include "Common.hpp"
#include "Logger.hpp"
#include "Side.hpp"
#include "UniqueObjectList.hpp"

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace boost;

static Logger::Handle logger(Logger::RequestHandle("Snake"));

// GAMECONSTANT: snake management constants
static const unsigned int defaultLength(90);
static const unsigned int snakeWidth(20);
static const unsigned int speedupPeriod(16000);
static const unsigned int speedupAmount(23);
static const unsigned int growthCap(100);
static const double linearGrowthRate(10.0 / 29.0);

Snake::Snake(Point center, UniqueObjectList& gameObjects)
{
	Init(center, gameObjects);
}

static void add_segment(Snake::Path& path, Point location, Direction direction, UniqueObjectList& gameObjects)
{
	SnakeSegment newSegment(location, direction, snakeWidth);

	for_each(path.begin(), path.end(), bind(&UniqueObjectList::remove, &gameObjects, _1));

	path.push_front(newSegment);

	for_each(path.begin(), path.end(), bind(&UniqueObjectList::add, &gameObjects, _1));
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

static inline Direction get_random_direction()
{
	const static Direction directions[] = {Direction::left, Direction::right, Direction::up, Direction::down};
	uint32_t randomNumber = minstd_rand(time(NULL))();
	return directions[randomNumber % countof(directions)];
}

void Snake::Init(Point center, UniqueObjectList& gameObjects)
{
	Point headLocation = center;

	moveTimer.Reset();
	speedupTimer.Reset();

	speed = 100;

	length = 0;
	projectedLength = defaultLength;

	add_segment(path, headLocation, get_random_direction(), gameObjects);
}

void Snake::Reset(Point center, UniqueObjectList& gameObjects)
{
	for_each(path.begin(), path.end(), bind(&UniqueObjectList::remove, &gameObjects, _1));
	path.clear();

	Init(center, gameObjects);
}
void Snake::ChangeDirection(Direction newDirection, UniqueObjectList& gameObjects)
{
	Direction direction(Head().GetDirection());

	if(newDirection != direction && newDirection != -direction && Head().GetWidth() >= 2 * snakeWidth)
	{
		// the point to start is the _direction_
		// side of the current head
		Bounds headBlock = Head().GetHeadSquare();
		// remove the head block from this segment
		Head().SetHeadSide(headBlock.GetSide(-direction));
		// take on the head block from the old segment
		Side startSide = headBlock.GetSide(-newDirection);

		add_segment(path, startSide.min, newDirection, gameObjects);
		// stretch this segment so that its initial size
		// is enough to cover the head block
		Head().SetHeadSide(headBlock.GetSide(newDirection));
	}
}

void Snake::Update(UniqueObjectList& gameObjects)
{
	for(Path::iterator i = path.begin(), end = path.end(); i != end; ++i)
	{
		if(i->GetDigestionInfo() != SnakeSegment::HUNGRY)
		{
			const double foodGrowthConstant = i->GetDigestionInfo();
			const double baseUncappedGrowth = projectedLength * linearGrowthRate;
			const double baseRealGrowth = min((double)growthCap, baseUncappedGrowth);

			const int growthAmount = round(baseRealGrowth * foodGrowthConstant);
			DEBUGLOG(logger, format("Growing by %1%") % growthAmount);
			Grow(growthAmount);

			i->Digest();
		}
	}

	while(speedupTimer.ResetIfHasElapsed(speedupPeriod))
		speed += speedupAmount;

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
				gameObjects.remove(Tail());
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
