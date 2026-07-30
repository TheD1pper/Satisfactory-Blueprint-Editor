#include <print>
#include <iostream>

#include "BenchmarkMacros.hpp"

import Helpers.Benchmark;
import Helpers.FsUtils;
import Core.Data;
import Parser.BinaryIO;
import Services.Load;
import Cli.Terminal;

int main()
{
	FsUtils::DirectoryCreateR(FsUtils::GetBenchmarkFolder());
	Benchmark::OverwriteSavePath(FsUtils::GetBenchmarkFolder());
	Services::LoadBlueprint(FsUtils::GetBlueprintsPath() / "Exp 1.2/Loop.sbp");
	Cli::PrintHeader(Services::LoadedBlueprint->Header);
	
} 