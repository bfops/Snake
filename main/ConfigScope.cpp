#include "ConfigScope.hpp"

#include "Logger.hpp"

// TODO: more error-checking

#include "ConfigScope.hpp"

#include "Logger.hpp"

static void register_value(ConfigScope::FieldMap& fields, const std::string& key,
	const std::string& value)
{
	if(fields.find(key) != fields.end())
		Logger::Debug(boost::format("Warning: field \"%1%\" already exists") % key);

	fields[key] = value;
}

static std::string get(std::istream& in)
{
	std::string gotten;
	in >> gotten;

	return gotten;
}

void ConfigScope::EnterScope(std::istream& in)
{
	const std::string scopeType = get(in);
	MemoryScopeList& memoryScopeList = subscopes[scopeType];
	ScopeList& scopes = memoryScopeList.first;
	// allocate a new subscope with this name
	scopes.push_back(ConfigScope(in));
	memoryScopeList.second = 0;
}

ConfigScope::ConfigScope(std::istream& in)
{
	while(!in.eof())
	{
		const std::string name = get(in);
		
		// start of scope
		if(name == "{")
		{
			EnterScope(in);
		}
		// end of scope
		else if(name == "}")
		{
			return;
		}
		else
		{
			const std::string value = get(in);

			register_value(fields, name, value);
		}
	}
}

bool ConfigScope::PeekScope(const std::string& name) const
{
	const ScopeMap::const_iterator index = subscopes.find(name);

	if(index == subscopes.end())
		return false;

	const ScopeList& scopes = index->second.first;
	const unsigned long currentScopeIndex = index->second.second;

	return (currentScopeIndex < scopes.size());
}

ConfigScope* ConfigScope::GetScope(const std::string& name)
{
	if(!PeekScope(name))
	{
		Logger::Debug(boost::format("Scope %1% not found to enter") % name);
		return NULL;
	}

	const ScopeMap::iterator index = subscopes.find(name);

	ScopeList& scopes = index->second.first;
	const unsigned long currentScopeIndex = index->second.second++;

	return &scopes[currentScopeIndex];
}
