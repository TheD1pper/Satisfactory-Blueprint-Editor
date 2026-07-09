#pragma once

#include <memory>

#include "Parser.hpp"

namespace Services
{
	extern std::unique_ptr<Core::Blueprint> LoadedBlueprint;
	void LoadBlueprint(const fs::path& _Path);
}