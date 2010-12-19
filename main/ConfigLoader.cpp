#include "ConfigLoader.hpp"

#include "Logger.hpp"

// TODO: more error-checking: incorrect number of params, incorrect close braces, etc.

void ConfigLoader::InitScopeStack()
{
	scopeStack.clear();
	scopeStack.push_back(&global);
}

ConfigLoader::ConfigLoader(std::istream& in) :
	global(in)
{
	InitScopeStack();
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
		Logger::Debug("Overexited scope!");
		return;
	}

	scopeStack.pop_back();
}

ConfigLoader::Scope& ConfigLoader::CurrentScope()
{
	return *scopeStack.back();
}
