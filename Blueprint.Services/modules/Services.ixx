module;

#include "Parser.hpp"

export module Services.Load;

import <memory>;

namespace Services
{
	export std::unique_ptr<Core::Blueprint> LoadedBlueprint = std::make_unique<Core::Blueprint>();
	export void LoadBlueprint(const fs::path& _Path);
}