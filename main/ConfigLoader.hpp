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

class ConfigLoader
{
public:
	// a { } scoped set of fields
	struct Scope
	{
		// essentially a vector of (fieldName, fieldValue) pairs
		typedef std::map<std::string, std::string> FieldMap;
		// a collection of same-typed scopes
		typedef std::vector<Scope> ScopeList;
		typedef std::pair<ScopeList, unsigned long> MemoryScopeList;
		typedef std::map<std::string, MemoryScopeList> ScopeMap;

		FieldMap fields;
		ScopeMap subscopes;

		Scope(std::istream&);

	private:
		void EnterScope(std::istream&);
	};

	typedef std::vector<Scope* const> ScopeStack;

private:
	Scope global;
	ScopeStack scopeStack;

	void InitScopeStack();

	Scope& CurrentScope();

public:
	ConfigLoader(std::istream& configInput);

	// for data-reading purposes
	bool EnterScope(const std::string& scopeName);
	void LeaveScope();
	
	// get _fieldName_'s value in the current scope and store in _dest_
	template <typename _T>
	void Get(const std::string& fieldName, _T& dest)
	{
		const Scope::FieldMap::const_iterator result = CurrentScope().fields.find(fieldName);

		if(result == CurrentScope().fields.end())
		{
			Logger::Debug(boost::format("Field %1% not found") % fieldName);
			return;
		}

		std::stringstream(result->second) >> dest;
	}
};
