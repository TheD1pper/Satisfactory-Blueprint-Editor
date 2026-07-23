#pragma once

#include <filesystem>
#include <fstream>
#include <ShlObj.h>
#include <Windows.h>

#include "Errors.hpp"

#pragma warning(disable : 4996) // Disable warning for getenv() use

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

	inline fs::path ResolveProgramFiles()
	{
		const char* GetenvResult = getenv("programfiles");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);

		return fs::path("");
	}

	inline fs::path ResolveAppdata()
	{
		const char* GetenvResult = getenv("appdata");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);

		return fs::path("");
	}

	inline fs::path ResolveLocalAppdata()
	{
		const char* GetenvResult = getenv("localappdata");
		if (GetenvResult && *GetenvResult)
			return fs::path(GetenvResult);

		return fs::path("");
	}

	inline fs::path GetSBELocalFolder()
	{
		return ResolveLocalAppdata() / "SatisfactoryBlueprintEditor";
	}

	inline fs::path GetBlueprintsPath()
	{
		return ResolveLocalAppdata() / "FactoryGame\\Saved\\SaveGames\\blueprints";
	};
	
	inline fs::path GetBenchmarkFolder()
	{
		return GetSBELocalFolder() / "tracing";
	}

	inline std::vector<fs::path> ScanDirectory(const fs::path& _Target)
	{
		std::vector<fs::path> Entries;
		for (const auto& Entry : fs::directory_iterator(_Target))
			Entries.push_back(Entry);
		return Entries;
	}

	
}