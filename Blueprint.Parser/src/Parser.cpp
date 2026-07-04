#include <iostream>
#include <print>

#include "Parser.hpp"
#include "BasicDataTypes.hpp"

namespace Parser
{
	InputBlueprint::InputBlueprint(fs::path& _Path)
	{
		Input = std::move(std::ifstream(std::move(_Path)));
	}

	template <>
	Result<Core::Int> InputBlueprint::Read()
	{
		Core::Int Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Int read"));
		return Value;
	}
	
	template <>
	Result<Core::Uint32> InputBlueprint::Read()
	{
		Core::Uint32 Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint32 read"));
		return Value;
	}

	template <>
	Result<Core::Uint64> InputBlueprint::Read()
	{
		Core::Uint64 Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 8))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint64 read"));
		return Value;
	}

	template <>
	Result<Core::String> InputBlueprint::Read()
	{
		auto r_Length = Read<Core::Int>();

		if (!r_Length.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad string length read"));

		Core::Int SignedLength = r_Length.value();

		if (SignedLength == 0)
			return std::unexpected(Eh::Error(Eh::Binary::EmptyString, "Empty string"));

		Core::String Value{};

		if (SignedLength > 0) // UTF-8
		{
			Value.Utf = Core::Encoding::Utf8;
			uint32_t Length = static_cast<uint32_t>(SignedLength);

			Value.Content = std::string(Length - 1, '\0');

			if (!ReadBytes(Value.Content.data(), Length - 1))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad u8string read"));

			SkipBytes(1); // Ignore null-termination
			return Value;
		}
		else if (SignedLength < 0) // UTF-16
		{
			Value.Utf = Core::Encoding::Utf16;
			uint32_t ByteLength = static_cast<uint32_t>(-SignedLength);

			Value.Content = std::string(ByteLength - 2, '\0');

			if (!ReadBytes(Value.Content.data(), ByteLength - 2))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad u16string read"));

			SkipBytes(2); // Ignore null-termination
			return Value;
		}

		return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad string length read"));
	}

	Result<Core::BlueprintHeader> InputBlueprint::ReadHeader()
	{
		Core::BlueprintHeader Draft;

		auto HeaderVersion = Read<Core::Uint32>();
		if (!HeaderVersion.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad header version read"));
		if (*HeaderVersion != 2)
			return std::unexpected(Eh::Error(Eh::Blueprint::WrongHeaderVersion, "Blueprint format is outdated")); // This parser currently can read version 2 blueprint file
		Draft.HeaderVersion = *HeaderVersion;

		auto SaveVersion = Read<Core::Uint32>();
		if (!SaveVersion.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save version read"));
		Draft.SaveVersion = SaveVersion.value();

		auto BuildVersion = Read<Core::Uint32>();
		if (!BuildVersion.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad build version read"));
		Draft.BuildVersion = BuildVersion.value();

		auto X = Read<Core::Uint32>();
		if (!X.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
		Draft.Size.X = X.value();

		auto Y = Read<Core::Uint32>();
		if (!Y.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size Y read"));
		Draft.Size.Y = Y.value();

		auto Z = Read<Core::Uint32>();
		if (!Z.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size Z read"));
		Draft.Size.Z = Z.value();

		auto r_NumberOfCostEntries = Read<Core::Uint32>();
		if (!r_NumberOfCostEntries.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of cost entries read"));
		Core::Uint32 NumberOfCostEntries = r_NumberOfCostEntries.value();
		if (NumberOfCostEntries <= 0)
			return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of cost entries can't be zero"));
		SkipBytes(4); // Ignore 4 empty bytes

		std::vector<Core::CostEntry> CostEntries;
		CostEntries.reserve(NumberOfCostEntries);
		for (uint32_t i = 0; i < NumberOfCostEntries; i++)
		{
			auto r_String = Read<Core::String>();
			if (!r_String.has_value())
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry class name read, iteration: " + static_cast<int>(i)));

			Core::CostEntry Draft;
			Draft.ClassName = *r_String;

			auto r_Quantity = Read<Core::Uint32>();
			if (!r_Quantity.has_value())
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry quantity read"));
			Draft.Quantity = *r_Quantity;
			if(i != NumberOfCostEntries - 1)
				SkipBytes(4); // Ignore 4 empty bytes

			CostEntries.emplace_back(Draft);
		}

		Draft.CostEntries = std::move(CostEntries);

		auto r_NumberOfContentEntries = Read<Core::Uint32>();
		if (!r_NumberOfContentEntries.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of content entries read"));
		Core::Uint32 NumberOfContentEntries = *r_NumberOfContentEntries;
		if (NumberOfContentEntries <= 0)
			return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of content entries can't be zero"));
		SkipBytes(4); // Ignore 4 empty bytes

		std::vector<Core::ContentEntry> ContentEntries;
		ContentEntries.reserve(NumberOfContentEntries);

		for (uint32_t i = 0; i < NumberOfContentEntries; i++)
		{
			Core::ContentEntry Draft;
			auto r_String = Read<Core::String>();
			if (!r_String.has_value())
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad content entry class name read"));
			Draft.ClassName = *r_String;
			ContentEntries.emplace_back(Draft);
			if (i != NumberOfContentEntries - 1)
				SkipBytes(4);
		}
		Draft.ContentEntries = std::move(ContentEntries);

		return Draft;
	}

	uint64_t InputBlueprint::GetBytesRead() const
	{
		return BytesRead;
	}

	void InputBlueprint::SkipBytes(std::streamsize _Bytes)
	{
		BytesRead += _Bytes;
		Input.ignore(_Bytes);

	}

	std::istream& InputBlueprint::ReadBytes(char* _String, uint64_t _Count)
	{
		Input.read(_String, _Count);
		BytesRead += Input.gcount();
		return Input;
	}
}