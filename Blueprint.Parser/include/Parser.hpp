#pragma once

#include <filesystem>
#include <fstream>
#include <optional>

namespace fs = std::filesystem;

namespace Parser
{
	class InputFile
	{
		std::ifstream Input;
		fs::path Path;

	public:
		InputFile(fs::path _Path);

		uint32_t Read();
	};

	fs::path GetLocalAppdata();
	fs::path GetBlueprintFolder();
	std::vector<fs::path> GetFolderContents(const fs::path& path);

	extern fs::path LocalAppData;
	extern fs::path BlueprintFolder;
}