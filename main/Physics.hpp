class GameWorld;
class UniqueObjectCollection;
class WorldObject;

namespace Physics
{
	// check all of _physicsObjects_ for collisions, and call respective collision handlers
	void Update(GameWorld& gameWorld, const UniqueObjectCollection& physicsObjects);
	// check if _obj_ collides with anything in _physicsObjects_
	bool AnyCollide(const WorldObject& obj, const UniqueObjectCollection& physicsObjects);
}
