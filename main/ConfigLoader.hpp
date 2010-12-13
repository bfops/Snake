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
	// essentially a vector of (fieldName, fieldData) pairs
	typedef std::map<const std::string, std::string> FieldMap;
	FieldMap fields;

	void Load(std::istream& configInput);

public:
	ConfigLoader(std::istream& configInput);

	// returns true iff _fieldName_ was found
	template <typename _T>
	bool Get(const std::string& fieldName, _T& dest) const
	{
		const FieldMap::const_iterator result = fields.find(fieldName);

		if(result == fields.end())
			return false;

		std::stringstream in(result->second);
		in >> dest;

		return true;
	}
};
