#include "ConfigLoader.hpp"

#include "Logger.hpp"

static Logger::Handle logger = Logger::RequestHandle("ConfigLoader");

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

		if(fields.find(key) == fields.end())
			fields[key] = value;
		else
			logger.Debug(boost::format("Field \"%1%\" already exists") % key);
	}
}
