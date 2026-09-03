module;
export module Helpers.FsUtils;

import std;

export namespace fs = std::filesystem;

namespace FsUtils
{
	export bool Exists(const fs::path& _Path);
	export bool IsFile(const fs::path& _Path);
	export bool isDirectory(const fs::path& _Path);
	export bool FileCreate(const fs::path& _Path);
	export bool Destroy(const fs::path& _Target);
	export bool DirectoryCreate(const fs::path& _Path);
	export bool DirectoryCreateR(const fs::path& _Path);
	export fs::path ResolveProgramFiles();
	export fs::path ResolveAppdata();
	export fs::path ResolveLocalAppdata();
	export fs::path ResolveTemp();
	export fs::path GetSBELocalFolder();
	export fs::path GetBlueprintsPath();
	export fs::path GetBenchmarkFolder();
	export fs::path GetTempFolder();
	export std::vector<fs::path> ScanDirectory(const fs::path& _Target);
}
