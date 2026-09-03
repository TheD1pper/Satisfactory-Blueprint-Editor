module;
#include <Windows.h>
module Helpers.FsUtils;

#pragma warning(disable : 4996) // Disable warning for getenv() use

import std;

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
		if (!Exists(_Path))
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

	bool DirectoryCreate(const fs::path& _Path)
	{
		return fs::create_directory(_Path);
	}

	bool DirectoryCreateR(const fs::path& _Path)
	{
		return fs::create_directories(_Path);
	}

	fs::path ResolveProgramFiles()
	{
		const char* GetenvResult = getenv("programfiles");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);
		return fs::path("");
	}

	fs::path ResolveAppdata()
	{
		const char* GetenvResult = getenv("appdata");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);
		return fs::path("");
	}

	fs::path ResolveLocalAppdata()
	{
		const char* GetenvResult = getenv("localappdata");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);
		return fs::path("");
	}

	fs::path ResolveTemp()
	{
		const char* GetenvResult = getenv("temp");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);
		return fs::path("");
	}

	fs::path GetSBELocalFolder()
	{
		return ResolveLocalAppdata() / "SatisfactoryBlueprintEditor";
	}

	fs::path GetBlueprintsPath()
	{
		return ResolveLocalAppdata() / "FactoryGame\\Saved\\SaveGames\\blueprints";
	}

	fs::path GetBenchmarkFolder()
	{
		return GetSBELocalFolder() / "tracing";
	}

	fs::path GetTempFolder()
	{
		return ResolveTemp() / "SatisfactoryBlueprintEditor";
	}

	std::vector<fs::path> ScanDirectory(const fs::path& _Target)
	{
		std::vector<fs::path> Entries;
		for (const auto& Entry : fs::directory_iterator(_Target))
			Entries.push_back(Entry);
		return Entries;
	}
}
