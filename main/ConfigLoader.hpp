#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#ifdef MSVC
#pragma warning(pop)
#endif

class ConfigLoader
{
private:
	typedef std::map<const std::string, std::string> FieldMap;
	FieldMap fields;

public:
	ConfigLoader();
	ConfigLoader(std::istream& configInput);

	void Load(std::istream& configInput);

	template <typename _T>
	void Pop(const std::string& fieldName, _T& dest) const
	{
		const FieldMap::const_iterator result = fields.find(fieldName);

		// TODO: error here
		if(result == fields.end())
			return;

		std::stringstream in(result->second);
		in >> dest;
	}
};
