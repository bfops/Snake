#include "Config.hpp"
#include "Logger.hpp"

// TODO: more error-checking

static std::string get(std::istream& in)
{
	std::string gotten;
	in >> gotten;

	return gotten;
}

Config::ConfigScope::ConfigScope(std::istream& in, long& bracketCount)
{
	Init(in, bracketCount);
}

void Config::ConfigScope::Init(std::istream& in, long& bracketCount)
{
	while(!in.eof())
	{
		const std::string name = get(in);
		
		// start of scope
		if(name == "{")
		{
			++bracketCount;
			EnterScope(in, bracketCount);
		}
		// end of scope
		else if(name == "}")
		{
			--bracketCount;
			return;
		}
		else
		{
			const std::string value = get(in);
			
			if(fields.find(name) != fields.end())
				Logger::Debug(boost::format("Warning: field \"%1%\" already exists") % name);

			fields[name] = value;
		}
	}
}

void Config::ConfigScope::EnterScope(std::istream& in, long& bracketCount)
{
	const std::string scopeType = get(in);
	MemoryScopeList& memoryScopeList = subscopes[scopeType];
	ScopeList& scopes = memoryScopeList.first;
	// allocate a new subscope with this name
	scopes.push_back(ConfigScope(in, bracketCount));
	memoryScopeList.second = 0;
}

Config::ConfigScope::ConfigScope(std::istream& in)
{
	long bracketCount = 0;
	Init(in, bracketCount);

	if(bracketCount > 0)
		Logger::Debug("Warning: Scope underterminated (not enough \"}\")");
	else if(bracketCount < 0)
		Logger::Debug("Warning: Scope overterminated (too many \"}\")");
}

bool Config::ConfigScope::PeekScope(const std::string& name) const
{
	const ScopeMap::const_iterator index = subscopes.find(name);

	if(index == subscopes.end())
		return false;

	const ScopeList& scopes = index->second.first;
	const unsigned long currentScopeIndex = index->second.second;

	return (currentScopeIndex < scopes.size());
}

Config::ConfigScope* Config::ConfigScope::GetScope(const std::string& name)
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
