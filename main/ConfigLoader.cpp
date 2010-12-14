#include "ConfigLoader.hpp"

#include "Logger.hpp"

static inline std::string get(std::istream& in)
{
	std::string gotten;
	in >> gotten;

	return gotten;
}

ConfigLoader::ConfigLoader(std::istream& in)
{
	Load(in);
}

void ConfigLoader::Load(std::istream& in)
{
	while(!in.eof())
	{
		const std::string key = get(in);
		const std::string value = get(in);

		if(fields.find(key) == fields.end())
			fields[key] = value;
		else
			Logger::Debug(boost::format("Field \"%1%\" already exists") % key);
	}
}
