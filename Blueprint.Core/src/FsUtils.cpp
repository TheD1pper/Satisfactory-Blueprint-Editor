#include <iostream>
#include <fstream>
#include <vector>

#include "FsUtils.hpp"

#pragma warning(disable : 4996) // Disable level 3 warning for getenv() use

namespace FsUtils
{
	bool Exists(const fs::path& _Path)
	{
		return fs::exists(_Path);
	}

	bool IsFile(const fs::path& _Path)
	{
		return fs::is_regular_file(_Path);
	}

	bool isDirectory(const fs::path& _Path)
	{
		return fs::is_directory(_Path);
	}

	bool FileCreate(const fs::path& _Path)
	{
		if(!Exists(_Path))
		{
			std::ofstream oFile(_Path);
			oFile.write("", 0);
		}
		if (Exists(_Path))
			return true;
		return false;
	}

	bool Destroy(const fs::path& _Target)
	{
		return fs::remove(_Target);
	}

	bool DirectoryCreate(const fs::path _Path)
	{
		return fs::create_directory(_Path);
	}

	bool DirectoryCreateR(const fs::path _Path)
	{
		return fs::create_directories(_Path);
	}

	fs::path ResolveAppdata()
	{
		return getenv("localappdata");
	}

	fs::path GetBlueprintsPath()
	{
		return Appdata.string() + "\\FactoryGame\\Saved\\SaveGames\\blueprints";
	}

	std::vector<fs::path> ScanDirectory(const fs::path& _Target)
	{
		std::vector<fs::path> Entries;
		for (const auto& Entry : fs::directory_iterator(_Target))
			Entries.push_back(Entry);
		return Entries;
	}

	const fs::path Appdata = ResolveAppdata();
	const fs::path BlueprintsPath = GetBlueprintsPath();
}

