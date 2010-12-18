#include "ConfigLoader.hpp"

#include "Logger.hpp"

// TODO: more error-checking: incorrect number of params, incorrect close braces, etc.

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

ConfigLoader::CommandMap ConfigLoader::DefaultCommandMap()
{
	ConfigLoader::CommandMap commandMap;
	commandMap["{"] = &ConfigLoader::EnterScope;
	commandMap["}"] = &ConfigLoader::LeaveScope;

	return commandMap;
}

void ConfigLoader::EnterScope(std::istream& in)
{
	const std::string scopeName = get(in);
	Scope::MemoryScopeList& memoryScopeList = CurrentScope().subscopes[scopeName];
	Scope::ScopeList& scopes = memoryScopeList.first;
	// allocate a new subscope with this name
	scopes.push_back(Scope());
	memoryScopeList.second = 0;
	// the new scope is now our focused scope
	scopeStack.push_back(&scopes.back());
}

void ConfigLoader::LeaveScope(std::istream&)
{
	if(scopeStack.size() <= 0)
	{
		Logger::Debug("Overterminated scope!");
		return;
	}

	scopeStack.pop_back();
}

void ConfigLoader::Load(std::istream& in)
{
	while(!in.eof())
	{
		const std::string command = get(in);
		
		const CommandMap::const_iterator index = commandMap.find(command);
		if(index == commandMap.end())
		{
			const std::string key = command;
			const std::string value = get(in);
			
			register_value(CurrentScope().fields, key, value);
		}
		else
		{
			// call the member function denoted in the map for this command
			if(index->second)
				(this->*(index->second))(in);
		}
	}

	if(scopeStack.size() > 1)
	{
		Logger::Debug("Improperly terminated scope!");
		InitScopeStack();
	}
}

void ConfigLoader::InitScopeStack()
{
	scopeStack.clear();
	scopeStack.push_back(&global);
}

ConfigLoader::ConfigLoader(std::istream& in) :
	commandMap(DefaultCommandMap())
{
	InitScopeStack();
	Load(in);
}

bool ConfigLoader::EnterScope(const std::string& name)
{
	const Scope::ScopeMap::iterator index = CurrentScope().subscopes.find(name);

	if(index == CurrentScope().subscopes.end())
		return false;

	Scope::ScopeList& scopes = index->second.first;
	const unsigned int currentScopeIndex = index->second.second++;

	if(currentScopeIndex >= scopes.size())
		return false;

	scopeStack.push_back(&scopes[currentScopeIndex]);
	return true;
}

void ConfigLoader::LeaveScope()
{
	if(scopeStack.size() <= 0)
	{
		Logger::Debug("Scope overexited");
		return;
	}

	scopeStack.pop_back();
}

ConfigLoader::Scope& ConfigLoader::CurrentScope()
{
	return *scopeStack.back();
}

const ConfigLoader::Scope& ConfigLoader::CurrentScope() const
{
	return *scopeStack.back();
}
