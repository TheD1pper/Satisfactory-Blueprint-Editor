#pragma once

#include <memory>

#include "Core.hpp"

import Parser.BinaryIO;

namespace Services
{
	extern std::unique_ptr<Core::Blueprint> LoadedBlueprint;
	void LoadBlueprint(const fs::path& _Path);
}