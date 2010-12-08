class GameWorld;
class UniqueObjectList;
class WorldObject;

namespace Physics
{
	void Update(GameWorld& gameWorld, UniqueObjectList& physicsObjects);
	// check if _obj_ collides with anything in _physicsObjects_
	bool AnyCollide(WorldObject& obj, UniqueObjectList& physicsObjects);
}
