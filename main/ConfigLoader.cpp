#include "ConfigLoader.hpp"

#include "Logger.hpp"

// TODO: more error-checking: incorrect number of params, incorrect close braces, etc.

static inline ConfigLoader::CommandMap default_command_map()
{
	ConfigLoader::CommandMap commandMap;
	commandMap["{"] = &ConfigLoader::StructCommand;
	commandMap["}"] = NULL;

	return commandMap;
}

static void register_value(ConfigLoader::FieldMap& fields, const std::string& key, const std::string& value)
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

ConfigLoader::ConfigLoader(std::istream& in) :
	commandMap(default_command_map())
{
	Load(in);
}

static inline std::string get_wall_data_name(const unsigned short i, const char* const specifier)
{
	std::stringstream s;
	s << "wall"
	  << i
	  << specifier;

	return s.str();
}

void ConfigLoader::StructCommand(std::istream& in)
{
	const std::string structType = get(in);
	if(structType == "wall")
	{
		static unsigned int nWalls = 0;
		register_value(fields, get_wall_data_name(nWalls, "MinX"), get(in));
		register_value(fields, get_wall_data_name(nWalls, "MinY"), get(in));
		register_value(fields, get_wall_data_name(nWalls, "MaxX"), get(in));
		register_value(fields, get_wall_data_name(nWalls, "MaxY"), get(in));
		++nWalls;
	}
	else
		Logger::Debug(boost::format("Warning: Struct type %1% not found") % structType);
}

void ConfigLoader::Load(std::istream& in)
{
	while(!in.eof())
	{
		const std::string command = get(in);
		
		// TODO: upper or lowercase search
		const CommandMap::const_iterator index = commandMap.find(command);
		if(index == commandMap.end())
		{
			const std::string key = command;
			const std::string value = get(in);
			
			register_value(fields, key, value);
		}
		else
		{
			// call the member function denoted in the map for this command
			if(index->second)
				(this->*(index->second))(in);
		}
	}
}
