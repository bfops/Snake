#include "ConfigLoader.hpp"

#include "Logger.hpp"

// TODO: more error-checking

void ConfigLoader::InitScopeStack()
{
	scopeStack.clear();
	scopeStack.push_back(&global);
}

ConfigLoader::Scope& ConfigLoader::CurrentScope()
{
	return *scopeStack.back();
}

const ConfigLoader::Scope& ConfigLoader::CurrentScope() const
{
	return *scopeStack.back();
}

ConfigLoader::ConfigLoader(std::istream& in) :
	global(in)
{
	InitScopeStack();
}

bool ConfigLoader::PeekScope(const std::string& name) const
{
	const Scope::ScopeMap::const_iterator index = CurrentScope().subscopes.find(name);

	if(index == CurrentScope().subscopes.end())
		return false;

	const Scope::ScopeList& scopes = index->second.first;
	const unsigned long currentScopeIndex = index->second.second;

	return (currentScopeIndex < scopes.size());
}

void ConfigLoader::EnterScope(const std::string& name)
{
	if(!PeekScope(name))
	{
		Logger::Debug(boost::format("Scope %1% not found to enter") % name);
		return;
	}

	const Scope::ScopeMap::iterator index = CurrentScope().subscopes.find(name);

	Scope::ScopeList& scopes = index->second.first;
	const unsigned long currentScopeIndex = index->second.second++;

	scopeStack.push_back(&scopes[currentScopeIndex]);
}

void ConfigLoader::LeaveScope()
{
	if(scopeStack.size() <= 0)
	{
		Logger::Debug("Overexited scope!");
		return;
	}

	scopeStack.pop_back();
}
