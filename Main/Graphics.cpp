#include "Graphics.hpp"

#include "Screen.hpp"
#include "UniqueObjectList.hpp"
#include "WorldObject.hpp"

#include <algorithm>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

namespace Graphics
{
	void Update(UniqueObjectList& graphicsObjects, Screen& target)
	{
		target.Clear();

		for_each(graphicsObjects.begin(), graphicsObjects.end(), bind(&WorldObject::Draw, _1, ref(target)));

		target.Update();
	}
}
