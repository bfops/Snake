#include "ConfigLoader.hpp"

#include "Logger.hpp"

static void register_value(ConfigLoader::Scope::FieldMap& fields, const std::string& key,
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

void ConfigLoader::Scope::EnterScope(std::istream& in)
{
	const std::string scopeType = get(in);
	Scope::MemoryScopeList& memoryScopeList = subscopes[scopeType];
	Scope::ScopeList& scopes = memoryScopeList.first;
	// allocate a new subscope with this name
	scopes.push_back(Scope(in));
	memoryScopeList.second = 0;
}

ConfigLoader::Scope::Scope(std::istream& in)
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
