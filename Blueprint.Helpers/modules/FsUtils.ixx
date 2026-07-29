module;

#include <Windows.h>

export module Helpers.FsUtils;

import <filesystem>;
import <fstream>;
import <ShlObj.h>;


#pragma warning(disable : 4996) // Disable warning for getenv() use

export namespace fs = std::filesystem;

namespace FsUtils
{
	export bool Exists(const fs::path& _Path)
	{
		return fs::exists(_Path);
	}

	export bool IsFile(const fs::path& _Path)
	{
		return fs::is_regular_file(_Path);
	}

	export bool isDirectory(const fs::path& _Path)
	{
		return fs::is_directory(_Path);
	}

	export bool FileCreate(const fs::path& _Path)
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

	export bool Destroy(const fs::path& _Target)
	{
		return fs::remove(_Target);
	}

	export bool DirectoryCreate(const fs::path _Path)
	{
		return fs::create_directory(_Path);
	}

	export bool DirectoryCreateR(const fs::path _Path)
	{
		return fs::create_directories(_Path);
	}

	export fs::path ResolveProgramFiles()
	{
		const char* GetenvResult = getenv("programfiles");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);
		return fs::path("");
	}

	export fs::path ResolveAppdata()
	{
		const char* GetenvResult = getenv("appdata");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);
		return fs::path("");
	}

	export fs::path ResolveLocalAppdata()
	{
		const char* GetenvResult = getenv("localappdata");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);
		return fs::path("");
	}

	export fs::path GetSBELocalFolder()
	{
		return ResolveLocalAppdata() / "SatisfactoryBlueprintEditor";
	}

	export fs::path GetBlueprintsPath()
	{
		return ResolveLocalAppdata() / "FactoryGame\\Saved\\SaveGames\\blueprints";
	};

	export fs::path GetBenchmarkFolder()
	{
		return GetSBELocalFolder() / "tracing";
	}

	export std::vector<fs::path> ScanDirectory(const fs::path& _Target)
	{
		std::vector<fs::path> Entries;
		for (const auto& Entry : fs::directory_iterator(_Target))
			Entries.push_back(Entry);
		return Entries;
	}
}