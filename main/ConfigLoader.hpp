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
		typedef std::vector<const std::string> Tuple;
		typedef std::pair<Tuple, unsigned long> MemoryTuple;
		// essentially a vector of (fieldName, fieldValues) pairs
		typedef std::map<const std::string, MemoryTuple> FieldMap;
		// a collection of same-typed scopes
		typedef std::vector<Scope> ScopeList;
		typedef std::pair<ScopeList, unsigned long> MemoryScopeList;
		typedef std::map<const std::string, MemoryScopeList> ScopeMap;

		FieldMap fields;
		ScopeMap subscopes;

		// returns true iff _fieldName_ was found
		template <typename _T>
		bool Get(const std::string& fieldName, _T& dest)
		{
			const FieldMap::iterator result = fields.find(fieldName);

			if(result == fields.end())
				return false;

			MemoryTuple& memoryTuple = result->second;
			const Tuple& tuple = memoryTuple.first;
			const unsigned long index = memoryTuple.second++;

			if(index >= tuple.size())
				return false;

			std::stringstream(tuple.front()) >> dest;

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
