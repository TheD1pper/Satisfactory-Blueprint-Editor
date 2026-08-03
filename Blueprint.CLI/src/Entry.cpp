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
	Services::BlueprintManager BpManager;
	FsUtils::DirectoryCreateR(FsUtils::GetBenchmarkFolder());
	Benchmark::OverwriteSavePath(FsUtils::GetBenchmarkFolder());
	auto r_Load = BpManager.Load(FsUtils::GetBlueprintsPath() / "Exp 1.2/Loop.sbp");
	if (!r_Load)
		std::print("Could not load the blueprint ({})", r_Load.error().GetLogMessage());
	else
	{
		Cli::PrintHeader(BpManager[*r_Load].Header);
		BpManager.Drop(*r_Load);
	}
} 