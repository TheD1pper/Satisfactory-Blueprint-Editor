module;

#include <vector>

#include "FsUtils.hpp"
#include "Core.hpp"

export module Cli.Terminal;

export namespace Cli
{
	export void ClearTerminal();
	export void ListAllDirectories(const fs::path& _Path);
	export void ListAllDirectories(const std::vector<fs::path>& _Contents);
	export void PrintHeader(const Core::BlueprintHeader& _Header);
}