#include "Snake.hpp"

#include "Common.hpp"
#include "Logger.hpp"

#include <boost/random.hpp>

using namespace std;
using namespace boost;

namespace {
Logger::Handle logger = Logger::RequestHandle("Snake");
DEF_CONSTANT(unsigned int, defaultLength, 90)
DEF_CONSTANT(unsigned int, snakeWidth, 20)
DEF_CONSTANT(unsigned int, speedupPeriod, 16000)
DEF_CONSTANT(unsigned int, speedupAmount, 23)
DEF_CONSTANT(unsigned int, growthCap, 200)
DEF_CONSTANT(double, linearGrowthRate, 10.0 / 29.0)
}

Snake::Snake(ObjectManager& objectManager)
{
	Reset(objectManager);
}

namespace {
void add_segment(Snake::Path& path, Point location, Direction direction, ObjectManager& objectManager)
{
	SnakeSegment* newSegment = objectManager.Create(SnakeSegment(location, direction, snakeWidth()));
	path.push_front(newSegment);
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
	return *path.front();
}
inline SnakeSegment& Snake::Tail()
{
	return *path.back();
}

Direction get_random_direction()
{
	const static Direction directions[] = {Direction::left(), Direction::right(), Direction::up(), Direction::down()};
	uint32_t randomNumber = minstd_rand(time(NULL))();
	return directions[randomNumber % countof(directions)];
}
void Snake::Reset(ObjectManager& objectManager)
{
	Point headLocation = objectManager.GetCenter();

	moveTimer.Reset();
	growTimer.Reset();
	speedupTimer.Reset();

	speed = 100;

	length = 0;
	projectedLength = defaultLength();
	path.clear();

	add_segment(path, headLocation, get_random_direction(), objectManager);
}
void Snake::ChangeDirection(Direction newDirection, ObjectManager& objectManager)
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

		add_segment(path, startSide.min, newDirection, objectManager);
	}
}

void Snake::Update(ObjectManager& objectManager)
{
	for(Path::iterator i = path.begin(), end = path.end(); i != end; ++i)
	{
		if((*i)->GetDigestionInfo() != SnakeSegment::HUNGRY)
		{
			double foodConstant = (*i)->GetDigestionInfo();
			const int uncappedGrowth = round(projectedLength * foodConstant * linearGrowthRate());
			int growthAmount;
			if(uncappedGrowth < 0)
				growthAmount = -min(growthCap(), (unsigned int)(-uncappedGrowth));
			else
				growthAmount = min(growthCap(), (unsigned int)(uncappedGrowth));

			logger.Debug(format("Growing by %1%") % growthAmount);
			Grow(growthAmount);

			(*i)->Digest();
		}
	}

	while(speedupTimer.ResetIfHasElapsed(speedupPeriod()))
		speed += speedupAmount();

	while(moveTimer.ResetIfHasElapsed(1000 / speed))
	{
		if(projectedLength < length)
		{
			--Tail();
			--length;
		}

		++Head();

		// when there is a request for more length,
		// simply don't shrink the tail
		if(length < projectedLength)
		{
			++length;
		}
		else
		{
			--Tail();

			if(Tail().IsEmpty())
			{
				objectManager.Destroy(&Tail());
				path.pop_back();
			}
		}
	}
}

bool Snake::IsDead() const
{
	return any(path.begin(), path.end(), boost::bind(&SnakeSegment::IsDead, _1));
}
