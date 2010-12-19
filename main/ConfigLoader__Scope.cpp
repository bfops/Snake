#include "ConfigLoader.hpp"

#include "Logger.hpp"

static void register_value(ConfigLoader::Scope::FieldMap& fields, const std::string& key, const std::string& value)
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

void ConfigLoader::Scope::EnterScope(std::istream& in)
{
	const std::string scopeName = get(in);
	Scope::MemoryScopeList& memoryScopeList = subscopes[scopeName];
	Scope::ScopeList& scopes = memoryScopeList.first;
	// allocate a new subscope with this name
	scopes.push_back(Scope(in));
	memoryScopeList.second = 0;
}

ConfigLoader::Scope::Scope(std::istream& in)
{
	while(!in.eof())
	{
		const std::string command = get(in);
		
		if(command == "{")
		{
			EnterScope(in);
		}
		else if(command == "}")
		{
			return;
		}
		else
		{
			const std::string key = command;
			const std::string value = get(in);
			
			register_value(fields, key, value);
		}
	}
}
