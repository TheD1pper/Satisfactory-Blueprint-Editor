#include <print>
#include <iostream>

#include "BenchmarkMacros.hpp"

import Helpers.Benchmark;
import Helpers.FsUtils;
import Core.Data;
import Parser.BinaryIO;
import Services.Manager;
import Cli.Terminal;

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	Services::BlueprintManager BpManager;
	FsUtils::DirectoryCreateR(FsUtils::GetBenchmarkFolder());
	Benchmark::OverwriteSavePath(FsUtils::GetBenchmarkFolder());
	auto r_Load = BpManager.Load(FsUtils::GetBlueprintsPath() / "Exp 1.2\\Loop.sbp");
	if (!r_Load)
		std::cerr << std::format("Could not load the blueprint ({})", r_Load.error().GetLogMessage());
	else
	{
		Cli::PrintHeader(BpManager[*r_Load].Header);
		Cli::PrintBody(BpManager[*r_Load].Body);
		BpManager.Drop(*r_Load);
	}
} 