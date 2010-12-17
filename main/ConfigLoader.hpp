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
public:
	// essentially a vector of (fieldName, fieldData) pairs
	typedef std::map<const std::string, std::string> FieldMap;

	typedef void (ConfigLoader::*CommandFunc)(std::istream& in);
	typedef std::map<const std::string, CommandFunc> CommandMap;

private:
	FieldMap fields;
	const CommandMap commandMap;

	void Load(std::istream& configInput);

public:
	ConfigLoader(std::istream& configInput);

	void StructCommand(std::istream&);

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
