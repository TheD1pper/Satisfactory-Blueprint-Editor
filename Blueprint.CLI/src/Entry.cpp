#include <print>
#include <iostream>

#include "Terminal.hpp"
#include "BenchmarkMacros.hpp"

import Helpers.Benchmark;
import Core.Data;
import Parser.BinaryIO;
import Services.Load;

int main()
{
	FsUtils::DirectoryCreateR(FsUtils::GetBenchmarkFolder());
	Benchmark::OverwriteSavePath(FsUtils::GetBenchmarkFolder());
	Services::LoadBlueprint(FsUtils::GetBlueprintsPath() / "Exp 1.2/Loop.sbp");
	Cli::PrintHeader(Services::LoadedBlueprint->Header);
	
} 