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

// TODO: square brackets for tuples
// TODO: named scopes (e.g. color scope named bgColor)
class ConfigLoader
{
public:
	// a { } scoped set of fields
	struct Scope
	{
		// essentially a vector of (fieldName, fieldData) pairs
		typedef std::map<const std::string, std::string> FieldMap;
		// a collection of similarly-typed scopes
		typedef std::vector<Scope> ScopeList;
		typedef std::pair<ScopeList, unsigned int> MemoryScopeList;
		typedef std::map<const std::string, MemoryScopeList> ScopeMap;

		FieldMap fields;
		ScopeMap subscopes;

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

	typedef void (ConfigLoader::*CommandFunc)(std::istream& in);
	typedef std::map<const std::string, CommandFunc> CommandMap;
	typedef std::vector<Scope* const> ScopeStack;

private:
	Scope global;
	ScopeStack scopeStack;

	const CommandMap commandMap;

	static CommandMap DefaultCommandMap();

	// for data-writing purposes
	void EnterScope(std::istream&);
	void LeaveScope(std::istream&);

	void Load(std::istream& configInput);
	void InitScopeStack();

public:
	ConfigLoader(std::istream& configInput);

	// for data-reading purposes
	bool EnterScope(const std::string& scopeName);
	void LeaveScope();
	
	Scope& CurrentScope();
	const Scope& CurrentScope() const;
};
