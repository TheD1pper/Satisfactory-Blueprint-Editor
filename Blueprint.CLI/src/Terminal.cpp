module;

#include <Windows.h>
#include <iostream>
#include <string>
#include <print>
#include <ctime>
#include <array>

module Cli.Terminal;

import Helpers.FsUtils;
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
		std::print("================ HEADER ================\n");
		std::print("\tHeader version: {}\n", _Header.HeaderVersion);
		std::print("\tSave version: {}\n", _Header.SaveVersion);
		std::print("\tBuild version: {}\n", _Header.BuildVersion);
		std::print("\tX: {}, Y: {}, Z: {}\n", _Header.Size.X, _Header.Size.Y, _Header.Size.Z);

		std::print("====== Cost Entries ======\n");
		for (auto& Entry : _Header.CostEntries)
			std::print("\t{} \n", Entry.ClassName.Content);

		std::print("====== Content Entries ======\n");
		for (auto& Entry : _Header.ContentEntries)
			std::print("\t{}\n", Entry.ClassName.Content);

		std::print("===== Misc Data =====\n");
		std::print("\tSave object data version: {}\n", _Header.SaveObjectDataVersion);
		std::print("\tPackage file version UE4: {}\n", _Header.PackageFileVersion.FileVersionUE4);
		std::print("\tPackage file version UE5: {}\n", _Header.PackageFileVersion.FileVersionUE5);
		std::print("\tLicensee version: {}\n", _Header.LicenseeVersion);
		std::print("===== Engine Version =====\n");
		std::print("\tMajor: {}\n", _Header.EngineVersion.Major);
		std::print("\tMinor: {}\n", _Header.EngineVersion.Minor);
		std::print("\tPatch: {}\n", _Header.EngineVersion.Patch);
		std::print("\tChange list: {}\n", _Header.EngineVersion.ChangeList);
		std::print("\tBranch: {}\n", _Header.EngineVersion.Branch.Content);
	}

	static void PrintRotation4D(const Core::Rotation4D& _Rotation)
	{
		std::print("\tX: {}, Y: {}, Z: {}, W: {}\n", _Rotation.X, _Rotation.Y, _Rotation.Z, _Rotation.W);
	}

	static void PrintPosition3D(const Core::Position3D& _Position)
	{
		std::print("\tX: {}, Y: {}, Z: {}\n", _Position.X, _Position.Y, _Position.Z);
	}

	static void PrintScale3D(const Core::Scale3D& _Scale)
	{
		std::print("\tX: {}, Y: {}, Z: {}\n", _Scale.X, _Scale.Y, _Scale.Z);
	}

	static void PrintActorHeader(const Core::ActorHeader& _Header)
	{
		std::print("\tType path: {}\n", _Header.TypePath.Content);
		std::print("\tRoot object: {}\n", _Header.RootObject.Content);
		std::print("\tInstance name: {}\n", _Header.InstanceName.Content);
		PrintRotation4D(_Header.Rotation);
		PrintPosition3D(_Header.Position);
		PrintScale3D(_Header.Scale);
		std::print("\tNeeds transformation: {}\n", _Header.NeedTransform);
		std::print("\tIs placed: {}\n", _Header.IsPlaced);
		std::print("\tUnknown: {}\n", _Header.Unknown);
	}

	static void PrintComponentHeader(const Core::ComponentHeader _Header)
	{
		std::print("\tType path: {}\n", _Header.TypePath.Content);
		std::print("\tRoot object: {}\n", _Header.RootObject.Content);
		std::print("\tInstance name: {}\n", _Header.InstanceName.Content);
		std::print("\tParent name: {}\n", _Header.ParentActorName.Content);
		std::print("\tUnknown: {}\n", _Header.Unknown);
	}

	void PrintBody(const Core::BlueprintBody& _Body)
	{
		std::print("================ BODY ================\n");
		std::print("\tUE package signature: {}\n", _Body.UEPackageSignature);


		std::print("===== Object Headers =====\n");
		for (const auto& Header : _Body.ObjectHeaders)
		{
			std::println();
			switch (Header.Type)
			{
			case Core::ObjectHeaderType::Actor:
				PrintActorHeader(std::get<Core::ActorHeader>(Header.Payload));
				break;

			case Core::ObjectHeaderType::Component:
				PrintComponentHeader(std::get<Core::ComponentHeader>(Header.Payload));
				break;
			}
		}

	}
}