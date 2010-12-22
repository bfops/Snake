#include "Graphics.hpp"

#include "Common.hpp"
#include "Screen.hpp"
#include "UniqueObjectList.hpp"
#include "WorldObject.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <algorithm>
#include <boost/bind.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

using namespace boost;

namespace Graphics
{
	void Update(const UniqueObjectList& graphicsObjects, const Screen& target)
	{
		target.Clear();

		for_each(graphicsObjects.begin(), graphicsObjects.end(), bind(&WorldObject::Draw, _1, ref(target)));

		target.Update();
	}
}
