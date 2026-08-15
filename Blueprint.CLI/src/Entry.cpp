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
	auto r_Load = BpManager.Load(FsUtils::GetBlueprintsPath() / "Exp 1.2\\Iron Ingot - Normal.sbp");
	if (!r_Load)
	{
		auto& Error = r_Load.error();
		auto Source = Error.GetSource();
		std::cerr << std::format("Could not load the blueprint ({}, file: {}, line: {})", Error.GetLogMessage(), static_cast<fs::path>(Source.file_name()).filename().string(), Source.line());
	}
	else
	{
		Cli::PrintHeader(BpManager[*r_Load].Header);
		Cli::PrintBody(BpManager[*r_Load].Body);
		BpManager.Drop(*r_Load);
	}
} 