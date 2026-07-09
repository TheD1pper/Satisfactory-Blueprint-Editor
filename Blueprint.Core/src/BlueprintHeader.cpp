#include "Core.hpp"

namespace Core
{
	BlueprintHeader::BlueprintHeader() // Constructor
	{

	}

	BlueprintHeader::BlueprintHeader(const BlueprintHeader& _Header) // Copy constructor
	{
		HeaderVersion = _Header.HeaderVersion;
		SaveVersion = _Header.SaveVersion;
		BuildVersion = _Header.BuildVersion;
		Size = _Header.Size;
		CostEntries = _Header.CostEntries;
		ContentEntries = _Header.ContentEntries;
		SaveObjectDataVersion = _Header.SaveObjectDataVersion;
		PackageFileVersion = _Header.PackageFileVersion;
		LicenseeVersion = _Header.LicenseeVersion;
		EngineVersion = _Header.EngineVersion;
	}

	BlueprintHeader::BlueprintHeader(BlueprintHeader&& _Header) noexcept // Move constructor
	{
		HeaderVersion = std::move(_Header.LicenseeVersion);
		SaveVersion = std::move(_Header.SaveVersion);
		BuildVersion = std::move(_Header.BuildVersion);
		Size = std::move(_Header.Size);
		CostEntries = std::move(_Header.CostEntries);
		ContentEntries = std::move(_Header.ContentEntries);
		SaveObjectDataVersion = std::move(_Header.SaveObjectDataVersion);
		PackageFileVersion = std::move(_Header.PackageFileVersion);
		LicenseeVersion = std::move(_Header.LicenseeVersion);
		EngineVersion = std::move(_Header.EngineVersion);
	}

	BlueprintHeader::~BlueprintHeader() // Default destructor
	{
		CostEntries = {};
		ContentEntries = {};
	}

	void BlueprintHeader::operator=(const BlueprintHeader& _Header) // Copy assigment operator
	{
		HeaderVersion = _Header.HeaderVersion;
		SaveVersion = _Header.SaveVersion;
		BuildVersion = _Header.BuildVersion;
		Size = _Header.Size;
		CostEntries = _Header.CostEntries;
		ContentEntries = _Header.ContentEntries;
		SaveObjectDataVersion = _Header.SaveObjectDataVersion;
		PackageFileVersion = _Header.PackageFileVersion;
		LicenseeVersion = _Header.LicenseeVersion;
		EngineVersion = _Header.EngineVersion;
	}

	void BlueprintHeader::operator=(BlueprintHeader& _Header) noexcept // Move operator
	{
		HeaderVersion = std::move(_Header.LicenseeVersion);
		SaveVersion = std::move(_Header.SaveVersion);
		BuildVersion = std::move(_Header.BuildVersion);
		Size = std::move(_Header.Size);
		CostEntries = std::move(_Header.CostEntries);
		ContentEntries = std::move(_Header.ContentEntries);
		SaveObjectDataVersion = std::move(_Header.SaveObjectDataVersion);
		PackageFileVersion = std::move(_Header.PackageFileVersion);
		LicenseeVersion = std::move(_Header.LicenseeVersion);
		EngineVersion = std::move(_Header.EngineVersion);
	}


	static_assert(std::is_copy_constructible_v <Core::BlueprintHeader>);
	static_assert(std::is_copy_assignable_v<Core::BlueprintHeader>);
	static_assert(std::is_move_constructible_v<Core::BlueprintHeader>);
	static_assert(std::is_move_assignable_v<Core::BlueprintHeader>);
	static_assert(std::is_destructible_v<Core::BlueprintHeader>);
}