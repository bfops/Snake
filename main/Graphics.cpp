#include "Graphics.hpp"

#include "Common.hpp"
#include "Screen.hpp"
#include "UniqueObjectCollection.hpp"
#include "WorldObject.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <algorithm>
#include <boost/bind.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

namespace Graphics
{
	void Update(const UniqueObjectCollection& graphicsObjects, const Screen& target)
	{
		target.Clear();

		for_each(graphicsObjects.begin(), graphicsObjects.end(),
			bind(&WorldObject::Draw, _1, boost::ref(target)));

		target.Update();
	}
}
