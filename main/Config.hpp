#pragma once

#include "Color24.hpp"
#include "Point.hpp"
#include "GameWorld.hpp"
#include "Logger.hpp"

struct Bounds;
class ConfigScope;

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <string>
#include <vector>

#ifdef MSVC
#pragma warning(pop)
#endif

// game configuration
struct Config
{
	// corresponds to a "scope" from the configuration file (see README)
	class ConfigScope
	{
	public:
		class ScopeCollection
		{
		public:
			// a simple collection of same-typed scopes
			typedef std::vector<ConfigScope> RawScopeCollection;

		private:
			RawScopeCollection collection;
			mutable unsigned long lastIndex;

		public:
			ScopeCollection();

			void Add(const ConfigScope& elem);
			// return true iff there is a scope to be gotten
			bool HasNextScope() const;
			// get the next scope, and advance the "next" pointer
			const ConfigScope& GetNextScope() const;
		};

		// map field name to field value
		typedef std::map<std::string, std::string> FieldMap;
		// map scope name to scope collection
		typedef std::map<std::string, ScopeCollection> ScopeMap;

	private:
		// _bracketCount_ is the count of curly braces
		ConfigScope(std::istream& configInput, long& bracketCount);

		void Init(std::istream& configInput, long& bracketCount);

		// recurse into another configuration scope
		void EnterScope(std::istream&, long& bracketCount);

		FieldMap fields;
		ScopeMap subscopes;

	public:
		ConfigScope(std::istream& configInput);

		bool PeekScope(const std::string& scopeName) const;
		const ConfigScope* GetScope(const std::string& scopeName) const;
	
		// get _fieldName_'s value in the current scope and store in _dest_
		template <typename _T>
		void GetField(const std::string& fieldName, _T& dest) const
		{
			const FieldMap::const_iterator result = fields.find(fieldName);

			if(result == fields.end())
			{
				Logger::Debug(boost::format("Field %1% not found") % fieldName);
				return;
			}

			std::stringstream(result->second) >> dest;
		}
		void GetField(const std::string& fieldName, Uint8& dest) const
		{
			unsigned short dummy;
			GetField(fieldName, dummy);
			dest = dummy;
		}
		void GetField(const std::string& fieldName, std::string& dest) const
		{
			const std::stringstream dummyStream;
			std::streambuf* const dummy = dummyStream.rdbuf();
			GetField(fieldName, dummy);
			dest = dummyStream.str();
		}
	};

private:
	Config(ConfigScope);

	static const Config gameConfig;
	static std::string GetDefaultConfig();

public:
	// recurses into the right config scope via construction
	struct ConfigLoadable
	{
		ConfigLoadable(const std::string& scopeName, const ConfigScope*& in);
	};

	template <typename _T>
	struct LoadableCollection : public ConfigLoadable
	{
		typedef std::vector<_T> Collection;
		typedef typename Collection::iterator iterator;
		typedef typename Collection::const_iterator const_iterator;

		Collection list;

		LoadableCollection(const std::string& listName, const std::string& elementsName, const ConfigScope* in) :
			ConfigLoadable(listName, in)
		{
			while(in->PeekScope(elementsName))
				list.push_back(_T(in));
		}
			
		iterator begin() { return list.begin(); }
		const_iterator begin() const { return list.begin(); }
		iterator end() { return list.end(); }
		const_iterator end() const { return list.end(); }
	};

	struct ColorConfig : public ConfigLoadable
	{
		Color24::ColorType r, g, b;

		ColorConfig(const ConfigScope* in);

		operator Color24() const;
	};

	struct BoundsConfig : public ConfigLoadable
	{
		Point min, max;

		BoundsConfig(const ConfigScope* in);

		operator Bounds() const;
	};

	struct SnakeConfig : public ConfigLoadable
	{
		struct Head : public ConfigLoadable
		{
			ColorConfig color;

			Head(const ConfigScope* in);
		};

		ColorConfig color;

		Head head;
		unsigned long startingLength;
		unsigned short width;
		unsigned short startingSpeed;

		unsigned int speedupPeriod;
		unsigned short speedupAmount;

		double growthRate;
		unsigned long growthCap;

		SnakeConfig(const ConfigScope* in);
	};

	// resource paths
	struct Resources : public ConfigLoadable
	{
		// SFX
		std::string eat;
		std::string spawn;
		std::string die;

		// musical
		std::string theme;

		Resources(const ConfigScope* in);
	};
	
	struct WallConfig : public ConfigLoadable
	{
		BoundsConfig bounds;
		ColorConfig color;

		WallConfig(const ConfigScope* in);
	};

	struct ScreenConfig : public ConfigLoadable
	{
		unsigned long w, h;
		ColorConfig bgColor;

		ScreenConfig(const ConfigScope* in);
	};

	struct SpawnCollectionConfig : public ConfigLoadable
	{
		struct SpawnConfig : public ConfigLoadable
		{
			ColorConfig color;
			// square size of spawn
			unsigned short size;
			// amount of empty space around spawns
			unsigned short cushion;
			// time before spawn disappears
			unsigned int expiry;
			// spawn rate
			double rate;

			SpawnConfig(const std::string& spawnScope, const ConfigScope*& in);

			// construct spawn from configuration data
			virtual GameWorld::SpawnPtr ConstructSpawn(Point location) const = 0;
		};

		struct FoodConfig : public SpawnConfig
		{
			long long points;
			double lengthFactor;
			short speedChange;

			FoodConfig(const ConfigScope* in);

			GameWorld::SpawnPtr ConstructSpawn(Point location) const;
		};

		struct MineConfig : public SpawnConfig
		{
			MineConfig(const ConfigScope* in);

			GameWorld::SpawnPtr ConstructSpawn(Point location) const;
		};

		typedef boost::shared_ptr<SpawnConfig> SpawnPtr;
		typedef std::vector<SpawnPtr> SpawnCollection;
		
		// spawn bounds
		BoundsConfig bounds;
		unsigned int period;
		SpawnCollection spawnsConfig;

		SpawnCollectionConfig(const ConfigScope* in);
	};

	// whether or not music/sound is on
	bool music, sound;

	unsigned short FPS;

	LoadableCollection<WallConfig> wallsConfig;
	ScreenConfig screen;
	SpawnCollectionConfig spawns;

	unsigned int pointGainPeriod;
	long long pointGainAmount;

	SnakeConfig snake;
	Resources resources;

	// get the (only) configuration data
	static const Config& Get();

	static const ConfigScope GetConfigLoader(const std::string& configFileName);
};
