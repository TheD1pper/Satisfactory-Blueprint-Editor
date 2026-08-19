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
	FsUtils::DirectoryCreateR(FsUtils::GetBenchmarkFolder()); // Create directory for benchmarks
	Benchmark::OverwriteSavePath(FsUtils::GetBenchmarkFolder()); // Bind directory for benchmarks

	fs::path BlueprintPath = Cli::GetBlueprintPath();

	auto r_Load = BpManager.Load(BlueprintPath);
	if (!r_Load)
	{
		auto& Error = r_Load.error();
		auto Source = Error.GetSource();
		std::cerr << std::format("Could not load the blueprint ({}, file: {}, line: {})", Error.GetLogMessage(), static_cast<fs::path>(Source.file_name()).filename().string(), Source.line());
		return 1;
	}

	Core::Blueprint& Blueprint = BpManager[*r_Load];

	Cli::PrintHeader(Blueprint.Header);
	Cli::PrintBody(Blueprint.Body);
	BpManager.Drop(Blueprint.Name);
} 