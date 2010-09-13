#include "World.hpp"
#include "WorldObject.hpp"
#include "collision.h"

#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/random.hpp>

#include "Common.hpp"
#include "custom_algorithm.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Timer.hpp"

using namespace std;
using namespace boost;

namespace World {
namespace {
typedef vector<WorldObject*> ObjectList;
ObjectList objects;
Logger::Handle logger = Logger::RequestHandle("World");

static inline bool object_exists(const WorldObject* obj)
{
	return in(objects.begin(), objects.end(), obj);
}

void Add(WorldObject& obj)
{
	assert(!object_exists(&obj));

	objects.push_back(&obj);
	logger.Debug(boost::format("Type %1% object %2% added") % obj.GetObjectType() % &obj);
}

void Remove(WorldObject& obj)
{
	if(unordered_find_and_remove(objects, &obj))
		logger.Debug(format("Type %1% object %2% removed") % obj.GetObjectType() % &obj);
	else
		logger.Debug(format("Invalid object (%1%) specified") % &obj);
}

namespace GameWorld {
namespace {
Timer foodTimer;
typedef vector<Food> Menu;
Menu foods;
DEF_CONSTANT(unsigned int, foodAdditionPeriod, 8000)
DEF_CONSTANT(unsigned int, foodSize, 15)
}
void Update(Point screenBounds)
{
	// URGENT URGENT TODO: fix this code!
	bool endReached = false;
	do
	{
		endReached = true;
		for(Menu::iterator i = foods.begin(), end = foods.end(); i != end; ++i)
		{
			if(i->IsEaten())
			{
				unordered_remove(foods, i);
				endReached = false;
				break;
			}
		}
	}
	while(!endReached);

	if(foodTimer.ResetIfHasElapsed(foodAdditionPeriod()))
	{
		// I'm 90% sure I'm doin' it wrong
		int32_t seed(time(NULL));
		Point foodLocation;
		foodLocation.x = minstd_rand0(seed)() % (screenBounds.x - foodSize());
		foodLocation.y = minstd_rand0(seed)() % (screenBounds.y - foodSize());

		Food newFood(foodLocation, foodSize());
		newFood.AddToWorld();
		foods.push_back(newFood);
	}
}
void Reset()
{
	foods.clear();
	foodTimer.Reset();
}
}

namespace GraphicsWorld {
// TODO: move the screen into GraphicsWorld entirely
void Update(Screen& target)
{
	target.Clear();

	std::for_each(objects.begin(), objects.end(),
		boost::bind(&WorldObject::Draw, _1, boost::ref(target))
	);

	target.Update();
}
}

namespace PhysicsWorld {
static inline CollidableObject world_to_collidable_object(const WorldObject* w)
{
	CollidableObject ret;

	Bounds bounds = w->GetBounds();
	ret.Min.x = bounds.min.x;
	ret.Min.y = bounds.min.y;
	ret.Max.x = bounds.max.x;
	ret.Max.y = bounds.max.y;

	return ret;
}

static inline void handle_potential_collision(WorldObject* o1, WorldObject* o2)
{
	CollidableObject c1 = world_to_collidable_object(o1);
	CollidableObject c2 = world_to_collidable_object(o2);

	if(does_collide(&c1, &c2))
	{
		o1->CollisionHandler(*o2);
		o2->CollisionHandler(*o1);
	}
}

static inline void collide_with_subsequent_objects(ObjectList::iterator collider)
{
	std::for_each(collider + 1, objects.end(), boost::bind(handle_potential_collision, *collider, _1));
}

void Update()
{
	for(ObjectList::iterator collider = objects.begin(), lastGroup = objects.end() - 1; collider != lastGroup; ++collider)
		collide_with_subsequent_objects(collider);
}
}
}

void Update(Screen& screen)
{
	PhysicsWorld::Update();
	GraphicsWorld::Update(screen);
	GameWorld::Update(screen.GetBounds());
}
void Reset()
{
	for_each(objects.begin(), objects.end(), bind(&WorldObject::RemoveFromWorld, _1));
	GameWorld::Reset();
}

WorldObject::WorldObject(ObjectType _type) :
	inWorld(false), type(_type)
{
}

WorldObject::WorldObject(const WorldObject& obj) :
	inWorld(obj.inWorld), type(obj.type), bounds(obj.bounds), color(obj.color)
{
	if(inWorld)
		World::Add(*this);
}

WorldObject& WorldObject::operator=(const WorldObject& o)
{
	if(!inWorld && o.inWorld)
		World::Add(*this);

	inWorld = o.inWorld;
	type = o.type;
	bounds = o.bounds;
	color = o.color;

	return *this;
}

WorldObject::~WorldObject()
{
	if(inWorld)
		World::Remove(*this);
}

void WorldObject::AddToWorld()
{
	if(!inWorld)
		World::Add(*this);
	else
		logger.Debug("Object already in world!");

	inWorld = true;
}
void WorldObject::RemoveFromWorld()
{
	if(inWorld)
		World::Remove(*this);
	else
		logger.Debug("Object not in world!");

	inWorld = false;
}

WorldObject::ObjectType WorldObject::GetObjectType() const
{
	return type;
}
void WorldObject::Draw(Screen& target) const
{
	SDL_Surface* surface = target.GetSurface();

	SDL_Rect rect;
	rect.w = bounds.max.x - bounds.min.x;
	rect.h = bounds.max.y - bounds.min.y;
	rect.x = bounds.min.x;
	rect.y = bounds.min.y;

	if(SDL_FillRect(surface, &rect, color.GetRGBMap(surface)) == -1)
	{
		string error = "Error drawing to screen: ";
		error += SDL_GetError();
		logger.Fatal(error.c_str());
	}
}

Bounds WorldObject::GetBounds() const
{
	return bounds;
}
Color24 WorldObject::GetColor() const
{
	return color;
}
}
