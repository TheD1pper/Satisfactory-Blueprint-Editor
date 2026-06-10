#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace Services
{
	std::string GetSavesString();
	std::string GetBlueprintsString(fs::path _Path);
}