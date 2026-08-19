module;

export module Cli.Terminal;

import <vector>;
import <filesystem>;

import Helpers.FsUtils;
import Core.Data;
import Core.Complex;

export namespace Cli
{
	export void ClearTerminal();
	export void ListAllDirectories(const fs::path& _Path);
	export void ListAllDirectories(const std::vector<fs::path>& _Contents);

	export fs::path GetBlueprintPath();

	export void PrintHeader(const Core::BlueprintHeader& _Header);
	export void PrintBody(const Core::BlueprintBody& _Body);
}