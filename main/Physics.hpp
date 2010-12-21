class GameWorld;
class UniqueObjectList;
class WorldObject;

namespace Physics
{
	// check all of _physicsObjects_ for collisions, and call respective collision handlers
	void Update(GameWorld& gameWorld, const UniqueObjectList& physicsObjects);
	// check if _obj_ collides with anything in _physicsObjects_
	bool AnyCollide(const WorldObject& obj, const UniqueObjectList& physicsObjects);
}
