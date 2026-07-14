#pragma once

#include <vector>

#include "BasicDataTypes.hpp"

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

		// ===== Decompressed variables =====

		Uint32 UncompressedBodySize{};
		Uint32 ObjectHeadersSize{};
		//std::vector<ObjectHeader> ObjectHeder;
		Uint32 ObjectsSize;
		//std::vector<Object> Objects;

	public:
		//BlueprintBody(); // Constructor
		//BlueprintBody(const BlueprintBody& _Body); // Copy contructor
		//BlueprintBody(BlueprintBody&& _Body) noexcept;
		//~BlueprintBody(); // Destructor

		//void operator=(const BlueprintBody& _Body); // Copy assigment operator
		//void operator=(BlueprintBody& _Body); // Move operator
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

		Uint32 UEPackageSignature{};
		Uint64 CompressedSize{};
		Uint64 UncompressedSize{};

	public:
		BlueprintHeader(); // Constructor
		BlueprintHeader(const BlueprintHeader& _Header); // Copy constructor
		BlueprintHeader(BlueprintHeader&& _Header) noexcept; // Move constructor
		~BlueprintHeader(); // Destructor

	public:
		void operator=(const BlueprintHeader& _Header); // Copy assigment operator
		void operator=(BlueprintHeader& _Header) noexcept; // Move operator
	};

	class Blueprint
	{
	public:
		BlueprintHeader Header;
		BlueprintBody Body;

	public:
		Blueprint() = default; // Default constructor
		Blueprint(BlueprintHeader& _Header, BlueprintBody& _Body) :
			Header(std::move(_Header)),
			Body(std::move(_Body)) {
		}
	};
}