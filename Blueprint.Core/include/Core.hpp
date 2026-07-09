#pragma once

#include <vector>

#include "BasicDataTypes.hpp"
#include "ObjectHeader.hpp"

namespace Core
{
	struct CostEntry
	{
		String ClassName{};
		Uint32 Quantity{};
	};

	struct ContentEntry
	{
		String ClassName{};
	};

	class BlueprintBody
	{
	public:
		Uint32 UEPackageSignature{};
		Uint64 CompressedSize{};
		Uint64 UncompressedSize{};
		Uint32 SizeOfObjects{}; // In number of bytes, including the number of objects but excluding this field

	};

	class BlueprintHeader
	{
	public:
		Uint32 HeaderVersion{};
		Uint32 SaveVersion{};
		Uint32 BuildVersion{};
		Size3D Size{};
		std::vector<CostEntry> CostEntries; 
		std::vector<ContentEntry> ContentEntries;

		Core::Uint32 SaveObjectDataVersion{};
		FPackageFileVersion PackageFileVersion{};
		Core::Int LicenseeVersion{};
		FEngineVersion EngineVersion{};

	public:
		BlueprintHeader() = default;
		BlueprintHeader(BlueprintHeader& _Header) = default;
		BlueprintHeader(BlueprintHeader&& other) noexcept = default;

	public:
		void Print();
	};

	class Blueprint
	{
	public:
		BlueprintHeader Header;
		BlueprintBody Body;

	public:
		Blueprint(BlueprintHeader& _Header, BlueprintBody& _Body) :
			Header(std::move(_Header)),
			Body(std::move(_Body)) {
		}
	};
}