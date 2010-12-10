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

	// returns true iff the field was found
	template <typename _T>
	bool Pop(const std::string& fieldName, _T& dest) const
	{
		const FieldMap::const_iterator result = fields.find(fieldName);

		if(result == fields.end())
			return false;

		std::stringstream in(result->second);
		in >> dest;

		return true;
	}
};
