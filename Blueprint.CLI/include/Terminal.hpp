#pragma once

import Helpers.FsUtils;

#include <vector>

#include "Core.hpp"

namespace Cli
{
	void ClearTerminal();
	void ListAllDirectories(const fs::path& _Path);
	void ListAllDirectories(const std::vector<fs::path>& _Contents);
	void PrintHeader(const Core::BlueprintHeader& _Header);
}