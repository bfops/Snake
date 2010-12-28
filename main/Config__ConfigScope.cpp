#include "Config.hpp"
#include "Logger.hpp"

// TODO: more error-checking

static std::string get(std::istream& in)
{
	std::string gotten;
	char c = '\0';
	do
	{
		if(in.eof())
			return gotten;

		in.get(c);
	}
	while(c == ' ' || c == '\n' || c == '\t');
	while(c != ' ' && c != '\n' && c != '\t')
	{
		gotten += c;
		
		if(in.eof())
			return gotten;

		in.get(c);
	}

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
	ScopeCollection& scopeCollection = subscopes[scopeType];
	// allocate a new subscope with this name
	scopeCollection.Add(ConfigScope(in, bracketCount));
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

	const ScopeCollection& scopeCollection = index->second;

	return scopeCollection.HasNextScope();
}

const Config::ConfigScope* Config::ConfigScope::GetScope(const std::string& name) const
{
	if(!PeekScope(name))
	{
		Logger::Debug(boost::format("Scope %1% not found to enter") % name);
		return NULL;
	}

	const ScopeMap::const_iterator index = subscopes.find(name);
	const ScopeCollection& scopeCollection = index->second;

	return &scopeCollection.GetNextScope();
}
