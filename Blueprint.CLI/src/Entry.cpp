#include <print>
#include <iostream>

#include "Terminal.hpp"
#include "Services.hpp"
#include "Parser.hpp"
#include "Benchmark.hpp"

int main()
{
	FsUtils::DirectoryCreateR(FsUtils::Appdata.string() + "\\SatisfactoryBlueprintEditor\\tracing\\");
	Benchmark::DefaultSavePath = FsUtils::Appdata.string() + "\\SatisfactoryBlueprintEditor\\tracing\\";
	Services::LoadBlueprint(FsUtils::BlueprintsPath.string() + "\\Exp 1.2\\Residual Fuel.sbp");
	Cli::PrintHeader(Services::LoadedBlueprint->Header);
	
} 