#include "ConfigLoader.hpp"

ConfigLoader::ConfigLoader()
{
}

ConfigLoader::ConfigLoader(std::istream& in)
{
	Load(in);
}

void ConfigLoader::Load(std::istream& in)
{
	while(!in.eof())
	{
		std::string key;
		std::string value;
		in >> key >> value;

		// TODO: check for overwrite first
		fields[key] = value;
	}
}
