#include <Windows.h>
#include <iostream>
#include <string>
#include <print>
#include <ctime>
#include <array>

#include "Services.hpp"
#include "Terminal.hpp"
#include "FsUtils.hpp"

import Core.Data;

namespace Cli
{
	void ClearTerminal()
	{
		system("cls");
	}

	void ListAllDirectories(const fs::path& _Path)
	{
		for (const auto& Entry : FsUtils::ScanDirectory(_Path))
		{	
			if (FsUtils::isDirectory(_Path))
				std::print("{}\n", Entry.filename().string());
		}
	}

	void ListAllDirectories(const std::vector<fs::path>& _Contents)
	{
		for (const auto& Entry : _Contents)
		{
			if (FsUtils::isDirectory(Entry))
				std::print("{}\n", Entry.filename().string());
		}
	}

	void PrintHeader(const Core::BlueprintHeader& _Header)
	{
		std::print("Header version: {}\n", _Header.HeaderVersion);
		std::print("Save version: {}\n", _Header.SaveVersion);
		std::print("Build version: {}\n", _Header.BuildVersion);
		std::print("X: {}, Y: {}, Z: {}\n", _Header.Size.X, _Header.Size.Y, _Header.Size.Z);

		std::print("======Cost Entries======\n");
		for (auto& Entry : _Header.CostEntries)
			std::print("\t{} \n", Entry.ClassName.Content);

		std::print("======Content Entries======\n");
		for (auto& Entry : _Header.ContentEntries)
			std::print("\t{}\n", Entry.ClassName.Content);

		std::print("=====Misc Data=====\n");
		std::print("\tSave object data version: {}\n", _Header.SaveObjectDataVersion);
		std::print("\tPackage file version UE4: {}\n", _Header.PackageFileVersion.FileVersionUE4);
		std::print("\tPackage file version UE5: {}\n", _Header.PackageFileVersion.FileVersionUE5);
		std::print("\tLicensee version: {}\n", _Header.LicenseeVersion);
		std::print("=====Engine Version=====\n");
		std::print("\tMajor: {}\n", _Header.EngineVersion.Major);
		std::print("\tMinor: {}\n", _Header.EngineVersion.Minor);
		std::print("\tPatch: {}\n", _Header.EngineVersion.Patch);
		std::print("\tChange list: {}\n", _Header.EngineVersion.ChangeList);
		std::print("\tBranch: {}\n", _Header.EngineVersion.Branch.Content);
	}
}