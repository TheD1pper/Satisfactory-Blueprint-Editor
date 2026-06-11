#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace Services
{
	[[nodiscard]] std::string GetSavesString();
	[[nodiscard]] std::string GetBlueprintsString(fs::path _Path);
}