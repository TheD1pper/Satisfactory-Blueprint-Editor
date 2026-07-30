module;

export module Services.Load;

import <memory>;
import <filesystem>;

import Core.Data;
import Parser.BinaryIO;

namespace Services
{
	export std::unique_ptr<Core::Blueprint> LoadedBlueprint = std::make_unique<Core::Blueprint>();
	export void LoadBlueprint(const fs::path& _Path);
}