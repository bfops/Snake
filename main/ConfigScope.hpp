#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef MSVC
#pragma warning(pop)
#endif

class ConfigScope
{
public:
	// essentially a vector of (fieldName, fieldValue) pairs
	typedef std::map<std::string, std::string> FieldMap;
	// a collection of same-typed scopes
	typedef std::vector<ConfigScope> ScopeList;
	typedef std::pair<ScopeList, unsigned long> MemoryScopeList;
	typedef std::map<std::string, MemoryScopeList> ScopeMap;

private:
	void EnterScope(std::istream&);

	FieldMap fields;
	ScopeMap subscopes;

public:
	ConfigScope(std::istream& configInput);

	// for data-reading purposes
	bool PeekScope(const std::string& scopeName) const;
	ConfigScope* GetScope(const std::string& scopeName);
	
	// get _fieldName_'s value in the current scope and store in _dest_
	template <typename _T>
	void Get(const std::string& fieldName, _T& dest) const
	{
		const FieldMap::const_iterator result = fields.find(fieldName);

		if(result == fields.end())
		{
			Logger::Debug(boost::format("Field %1% not found") % fieldName);
			return;
		}

		std::stringstream(result->second) >> dest;
	}
};
