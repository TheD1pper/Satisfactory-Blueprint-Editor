#pragma once

#include <vector>

#include "BasicDataTypes.hpp"

namespace Core
{
	struct CostEntry
	{
		String ClassName{};
		uint32_t Quantity{};
	};

	struct ContentEntry
	{
		String ClassName{};
	};

	class BlueprintBody
	{
	public:
		uint32_t UEPackageSignature{};
		uint64_t CompressedSize{};
		uint64_t UncompressedSize{};

		// ===== Decompressed variables =====

		uint32_t UncompressedBodySize{};
		uint32_t ObjectHeadersSize{};
		//std::vector<ObjectHeader> ObjectHeder;
		uint32_t ObjectsSize;
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
		uint32_t HeaderVersion{};
		uint32_t SaveVersion{};
		uint32_t BuildVersion{};
		Size3D Size{};
		std::vector<CostEntry> CostEntries; 
		std::vector<ContentEntry> ContentEntries;

		uint32_t SaveObjectDataVersion{};
		FPackageFileVersion PackageFileVersion{};
		int LicenseeVersion{};
		FEngineVersion EngineVersion{};

		uint32_t UEPackageSignature{};
		uint64_t CompressedSize{};
		uint64_t UncompressedSize{};

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