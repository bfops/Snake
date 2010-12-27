#include "Config.hpp"

Config::ConfigScope::ScopeCollection::ScopeCollection()
{
	lastIndex = 0;
}

void Config::ConfigScope::ScopeCollection::Add(const Config::ConfigScope& elem)
{
	collection.push_back(elem);
}

bool Config::ConfigScope::ScopeCollection::HasNextScope() const
{
	return (lastIndex < collection.size());
}

const Config::ConfigScope& Config::ConfigScope::ScopeCollection::GetNextScope() const
{
	return collection[lastIndex++];
}
