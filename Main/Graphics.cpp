#include "Graphics.hpp"

#include <algorithm>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

namespace Graphics
{
	void Update(UniqueObjectList& gameObjects, Screen& target)
	{
		target.Clear();

		for_each(gameObjects.begin(), gameObjects.end(), bind(&WorldObject::Draw, _1, ref(target)));

		target.Update();
	}
}