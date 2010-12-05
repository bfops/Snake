#pragma once

#include "WorldObject.hpp"

// TODO: come up with a different sentinel collision
// mechanism; using the physics engine as-is will
// start to become cumbersome
class Sentinel : public WorldObject
{
private:
	bool interfering;

	void Interfere();
	void Init();

public:
	Sentinel(Point location, unsigned int size);
	
	void CollisionHandler(WorldObject&) const;
	void CollisionHandler(const Food&);
	void CollisionHandler(const Mine&);
	void CollisionHandler(const SnakeSegment&);
	void CollisionHandler(const Sentinel&);
	void CollisionHandler(const Wall&);

	bool IsInterfering() const;
};
