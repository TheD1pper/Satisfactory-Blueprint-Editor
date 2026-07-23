#include <print>
#include <iostream>

#include "Terminal.hpp"
#include "Services.hpp"
#include "Parser.hpp"
#include "Benchmark.hpp"

int main()
{
	FsUtils::DirectoryCreateR(FsUtils::GetBenchmarkFolder());
	Benchmark::OverwriteSavePath(FsUtils::GetBenchmarkFolder());
	Services::LoadBlueprint(FsUtils::GetBlueprintsPath() / "Exp 1.2/Loop.sbp");
	Cli::PrintHeader(Services::LoadedBlueprint->Header);
	
} 