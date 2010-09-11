#include "World.hpp"
#include "WorldObject.hpp"

#include <vector>
#include <boost/bind/bind.hpp>

#include "custom_algorithm.hpp"
#include "Logger.hpp"

using namespace std;
using namespace boost;

namespace World {
namespace {
typedef vector<WorldObject*> ObjectList;
ObjectList objects;
Logger::Handle logger = Logger::RequestHandle("World");

static inline bool object_exists(const WorldObject* obj)
{
	return find(objects.begin(), objects.end(), obj) != objects.end();
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
		return logger.Debug(boost::format("Type %1% object %2% removed") % obj.GetObjectType() % &obj);

	logger.Debug("Invalid object specified");
}

namespace GraphicsWorld {
// TODO: just move the damn screen in?
void Update(Screen& target)
{
	target.Clear();

	for(ObjectList::iterator i = objects.begin(), end = objects.end(); i != end; ++i)
		(*i)->Draw(target);

	target.Update();
}
}

namespace PhysicsWorld {
static inline bool IsCollide(const WorldObject* obj1, const WorldObject* obj2)
{
#define TESTBOUNDS(m) ((obj1Min.m <= obj2Min.m) ? (obj2Min.m < obj1Max.m) : (obj1Min.m < obj2Max.m))
	const Point obj1Min = obj1->GetMinBounds();
	const Point obj1Max = obj1->GetMaxBounds();
	const Point obj2Min = obj2->GetMinBounds();
	const Point obj2Max = obj2->GetMaxBounds();
	return (TESTBOUNDS(x) && TESTBOUNDS(y));
#undef TESTBOUNDS
}

static inline void handle_potential_collision(WorldObject* o1, WorldObject* o2)
{
	if(IsCollide(o1, o2))
	{
		o1->CollisionHandler(*o2);
		o2->CollisionHandler(*o1);
	}
}

static inline void collide_with_subsequent_objects(ObjectList::iterator collider)
{
	for(ObjectList::iterator collidee = collider + 1, lastGroup = objects.end(); collidee != lastGroup; ++collidee)
		handle_potential_collision(*collider, *collidee);
}

void Update()
{
	for(ObjectList::iterator collider = objects.begin(), lastGroup = objects.end() - 1; collider != lastGroup; ++collider)
		collide_with_subsequent_objects(collider);
}
}
}

void Update(Screen& target)
{
	PhysicsWorld::Update();
	GraphicsWorld::Update(target);
}

WorldObject::WorldObject(ObjectType _type) :
	inWorld(false), type(_type)
{
}
WorldObject::WorldObject(const WorldObject& obj) :
	inWorld(obj.inWorld), minBounds(obj.minBounds), maxBounds(obj.maxBounds), color(obj.color)
{
	if(inWorld)
		World::Add(*this);
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

	inWorld = true;
}
void WorldObject::RemoveFromWorld()
{
	if(inWorld)
		World::Remove(*this);

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
	rect.w = maxBounds.x - minBounds.x;
	rect.h = maxBounds.y - minBounds.y;
	rect.x = minBounds.x;
	rect.y = minBounds.y;

	if(SDL_FillRect(surface, &rect, color.GetRGBMap(surface)) == -1)
	{
		string error = "Error drawing to screen: ";
		error += SDL_GetError();
		logger.Fatal(error.c_str());
	}
}

Point WorldObject::GetMinBounds() const
{
	return minBounds;
}
Point WorldObject::GetMaxBounds() const
{
	return maxBounds;
}
Color24 WorldObject::GetColor() const
{
	return color;
}
}
