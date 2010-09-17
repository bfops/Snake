#pragma once

#include "Bounds.hpp"
#include "Logger.hpp"
#include "Point.hpp"
#include "WorldObject.hpp"

#include "custom_algorithm.hpp"

#include <boost/shared_ptr.hpp>
#include <memory>
#include <vector>

class World
{
public:
	typedef std::vector<boost::shared_ptr<WorldObject> > ObjectList;

private:
	ObjectList objects;
	Logger::Handle logger;

public:
	World();

	void Update();
	void Reset();
	Point GetCenter() const;

	/// re-creates an object passed and adds
	/// the new one to the world
	template <typename _T>
	_T* Create(const _T& obj)
	{
		_T* newObject = new _T(obj);
		boost::shared_ptr<WorldObject> ptr(newObject);
		objects.push_back(ptr);

		return newObject;
	}
	template <typename _T>
	void Destroy(_T* obj)
	{
		// TODO: re-map / unordered_find_and_remove
		for(ObjectList::iterator i = objects.begin(), end = objects.end(); i != end; ++i)
		{
			if(i->get() == obj)
			{
				objects.erase(i);
				break;
			}
		}
	}
};
