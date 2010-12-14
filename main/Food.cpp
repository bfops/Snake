#include "Food.hpp"

#include "Common.hpp"
#include "Sentinel.hpp"

const Food::FoodInfo Food::ice(-2.5, Color24(0, 0, 255), -300);
const Food::FoodInfo Food::celery(0.3, Color24(127, 255, 127), 25);
const Food::FoodInfo Food::normal(1.0, Color24(0, 255, 255), 100);
const Food::FoodInfo Food::donut(3.0, Color24(200, 0, 0), 400);

Food::Food(const Sentinel& prototype, const unsigned int size, const FoodInfo& foodInfo) :
	WorldObject(food, foodInfo.color)
{
	calories = foodInfo.calories;
	pointsGiven = foodInfo.pointsGiven;

	// prototypes are guaranteed to be squares, so \Delta x = \Delta y
	const unsigned int prototypeSize = prototype.GetBounds().max.x - prototype.GetBounds().min.x;
	const unsigned int sizeDiff = prototypeSize - size;
	bounds.min = prototype.GetBounds().min;
	bounds.min.x += sizeDiff / 2;
	bounds.min.y += sizeDiff / 2;
	bounds.max = bounds.min;
	bounds.max.x += size;
	bounds.max.y += size;
}

void Food::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

double Food::GetCalories() const
{
	return calories;
}

int Food::GetPoints() const
{
	return pointsGiven;
}
