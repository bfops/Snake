#include "World.hpp"
#include "WorldObject.hpp"

#include <vector>

#include "collision.h"
#include "Common.hpp"
#include "custom_algorithm.hpp"
#include "Food.hpp"
#include "Logger.hpp"
#include "Screen.hpp"
#include "Timer.hpp"
#include "Wall.hpp"

using namespace std;
using namespace boost;

namespace {
DEF_CONSTANT(Point, screenBounds, Point(800, 600))
inline bool object_exists(const WorldObject* obj, World::ObjectList& objects)
{
	// TODO: use in()
	for(World::ObjectList::iterator i = objects.begin(), end = objects.end(); i != end; ++i)
		if(obj == i->get())
			return true;

	return false;
}

namespace GraphicsWorld {
namespace {
Screen screen(screenBounds().x, screenBounds().y);
}

void Update(World::ObjectList& objects)
{
	screen.Clear();

	// TODO: re-bind
	for(World::ObjectList::iterator i = objects.begin(), end = objects.end(); i != end; ++i)
		(*i)->Draw(screen);

	screen.Update();
}
}

namespace PhysicsWorld {
namespace {
inline CollidableObject world_to_collidable_object(const WorldObject* w)
{
	CollidableObject ret;

	Bounds bounds = w->GetBounds();
	ret.min.x = bounds.min.x;
	ret.min.y = bounds.min.y;
	ret.max.x = bounds.max.x;
	ret.max.y = bounds.max.y;

	return ret;
}

void handle_potential_collision(WorldObject* o1, WorldObject* o2)
{
	CollidableObject c1 = world_to_collidable_object(o1);
	CollidableObject c2 = world_to_collidable_object(o2);

	if(does_collide(&c1, &c2))
	{
		o1->CollisionHandler(*o2);
		o2->CollisionHandler(*o1);
	}
}

inline void collide_with_subsequent_objects(World::ObjectList::iterator collider, World::ObjectList::iterator end)
{
	// TODO: re-bind
	for(World::ObjectList::iterator collidee = collider + 1; collidee != end; ++collidee)
		handle_potential_collision(collider->get(), collidee->get());
}

}

void Update(World::ObjectList& objects)
{
	// don't try the last objects, since all have been checked against it
	for(World::ObjectList::iterator collider = objects.begin(), end = objects.end() - 1; collider != end; ++collider)
		collide_with_subsequent_objects(collider, objects.end());
}
}
}

World::World() :
	logger(Logger::RequestHandle("World"))
{
	Reset();
}

void World::Update()
{
	PhysicsWorld::Update(objects);
	GraphicsWorld::Update(objects);
}

void World::Reset()
{
	objects.clear();
}

Point World::GetCenter() const
{
	return Point(screenBounds().x / 2, screenBounds().y / 2);
}
