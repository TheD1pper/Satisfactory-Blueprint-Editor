#pragma once

#include <filesystem>

#include "Errors.hpp"

namespace fs = std::filesystem;

namespace FsUtils
{
	bool Exists(const fs::path& _Path);
	bool IsFile(const fs::path& _Path);
	bool isDirectory(const fs::path& _Path);
	bool FileCreate(const fs::path& _Path);

	bool Destroy(const fs::path& _Target);
	bool DirectoryCreate(const fs::path _Path);
	bool DirectoryCreateR(const fs::path _Path);
	fs::path ResolveAppdata();
	fs::path GetBlueprintsPath();
	std::vector<fs::path> ScanDirectory(const fs::path& _Target);

	extern const fs::path Appdata;
	extern const fs::path BlueprintsPath;
}