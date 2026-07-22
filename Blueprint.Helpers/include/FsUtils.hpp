#pragma once

#include <filesystem>
#include <fstream>

#include "Errors.hpp"

#pragma warning(disable : 4996) // Disable level 3 warning for getenv() use

namespace fs = std::filesystem;

namespace FsUtils
{
	inline bool Exists(const fs::path& _Path)
	{
		return fs::exists(_Path);
	}

	inline bool IsFile(const fs::path& _Path)
	{
		return fs::is_regular_file(_Path);
	}

	inline bool isDirectory(const fs::path& _Path)
	{
		return fs::is_directory(_Path);
	}

	inline bool FileCreate(const fs::path& _Path)
	{
		if (!Exists(_Path))
		{
			std::ofstream oFile(_Path);
			oFile.write("", 0);
		}
		if (Exists(_Path))
			return true;
		return false;
	}

	inline bool Destroy(const fs::path& _Target)
	{
		return fs::remove(_Target);
	}

	inline bool DirectoryCreate(const fs::path _Path)
	{
		return fs::create_directory(_Path);
	}

	inline bool DirectoryCreateR(const fs::path _Path)
	{
		return fs::create_directories(_Path);
	}

	inline fs::path ResolveAppdata()
	{
		return getenv("localappdata");
	}

	inline fs::path Appdata = ResolveAppdata();

	inline fs::path GetBlueprintsPath()
	{
		return Appdata.string() + "\\FactoryGame\\Saved\\SaveGames\\blueprints";
	}

	inline std::vector<fs::path> ScanDirectory(const fs::path& _Target)
	{
		std::vector<fs::path> Entries;
		for (const auto& Entry : fs::directory_iterator(_Target))
			Entries.push_back(Entry);
		return Entries;
	}

	inline fs::path BlueprintsPath = GetBlueprintsPath();
}