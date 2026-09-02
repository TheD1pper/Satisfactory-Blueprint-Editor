module;

#include <cassert>

module Parser.Blueprint:Input;

namespace Eh = ErrorHandling;

using namespace Core;

namespace Parser
{
#pragma region BlueprintInput 
	BlueprintInput::BlueprintInput(const fs::path& _Path) :
		m_Input(_Path, std::ios::binary) {
	}

	uint64 BlueprintInput::GetPtrPos()
	{
		return m_Input.tellg();
	}

	std::istream& BlueprintInput::ReadBytes(char* _Ptr, uint64 _Count)
	{
		m_Input.read(_Ptr, _Count);
		return m_Input;
	}

	inline void BlueprintInput::SkipBytes(uint64 _Count)
	{
		m_Input.ignore(_Count);
	}
#pragma endregion implementations

#pragma region Read()
	template<typename T>
	Result<T> BlueprintInput::Read()
	{
		static_assert(std::is_arithmetic_v<T>, "T is not trivial; provide an explicit BlueprintInput::Read<T>() specialization");

		if (T Value{}; !ReadBytes(reinterpret_cast<char*>(&Value), sizeof(T)))
			return MakeError(Eh::Binary::BadRead);
		else
			return Value;
	}

	template<>
	Result<String> BlueprintInput::Read()
	{
		String Draft;

		SkipBytes(4);

		auto r_Length = Read<int32>();
		if (!r_Length)
			return MakeError(Eh::Binary::BadRead, "Could not read string length");

			if (*r_Length == 0)
				return MakeError(Eh::Binary::EmptyString, "Empty string");

			if (*r_Length > 0)
			{
				std::string Data;
				Data.resize(*r_Length);

				if (!ReadBytes(Data.data(), *r_Length))
					return MakeError(Eh::Binary::BadRead, "Could not read UTF-8 string data");

				SkipBytes(1); // Skip null-terminator

				Draft.Data = std::move(Data);
				Draft.Encoding = StrEncoding::Utf8;
			}

			if (*r_Length < 0)
			{
				assert(true);
				std::wstring Data;
				Data.resize(*r_Length);

				if (!ReadBytes(reinterpret_cast<char*>(Data.data()), *r_Length))
					return MakeError(Eh::Binary::BadRead, "Could not read UTF-16 string data");

				SkipBytes(2); // Skip null-terminator

				Draft.Data = std::move(Data);
				Draft.Encoding = StrEncoding::Utf16;
			}

			return Draft;
	}

	template<>
	Result<std::array<float32, 3>> BlueprintInput::Read()
	{
		std::array<float32, 3> Draft{};

		for (size_t i = 0; i < 3; i++)
		{
			auto r_Value = Read<float32>();
			if (!r_Value)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read value of index: {}", i));

			Draft[i] = *r_Value;
		}

		return Draft;
	}

	template<>
	Result<std::array<int32, 3>> BlueprintInput::Read()
	{
		std::array<int32, 3> Draft{};

		for (size_t i = 0; i < 3; i++)
		{
			auto r_Value = Read<int32>();
			if (!r_Value)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read value of index: {}", i));

			Draft[i] = *r_Value;
		}

		return Draft;
	}

	template<>
	Result<ObjectReference> BlueprintInput::Read()
	{
		ObjectReference Draft;

		auto r_Level = Read<String>();
		if (!r_Level)
			return MakeError(Eh::Binary::BadRead, "Could not read reference level");
		Draft.Level = std::move(*r_Level);

		SkipBytes(4);

		auto r_Path = Read<String>();
		if (!r_Path)
			return MakeError(Eh::Binary::BadRead, "Could not read reference path");
		Draft.Path = std::move(*r_Path);

		return Draft;
	}

	template<>
	Result<std::vector<CostEntry>> BlueprintInput::Read()
	{
		std::vector<CostEntry> Draft;

		auto r_Count = Read<uint32>();
		if (!r_Count)
			return MakeError(Eh::Binary::BadRead, "Could not read count of cost entries");
		Draft.resize(*r_Count);

		for (size_t i = 0; i < *r_Count; i++)
		{
			auto r_Class = Read<ObjectReference>();
			if (!r_Class)
				return MakeError(Eh::Binary::BadRead, std::format("Could not class name of index: {}", i));
			Draft[i].Class = std::move(*r_Class);

			auto r_Amount = Read<uint32>();
			if (!r_Amount)
				return MakeError(Eh::Binary::BadRead, std::format("Could not amount of index: {}", i));
			Draft[i].Amount = std::move(*r_Amount);
		}

		return Draft;
	}

	template<>
	Result<std::vector<ObjectReference>> BlueprintInput::Read()
	{
		std::vector<ObjectReference> Draft;

		auto r_Count = Read<uint32>();
		if(!r_Count)
			return MakeError(Eh::Binary::BadRead, "Could not read count of recipes");
		Draft.resize(*r_Count);

		for (size_t i = 0; i < *r_Count; i++)
		{
			auto r_Entry = Read<ObjectReference>();
			if (!r_Entry)
				return MakeError(Eh::Binary::BadRead, "Could not read a recipe");
			Draft[i] = std::move(*r_Entry);
		}

		return Draft;
	}

	template<>
	Result<PackageFileVersion> BlueprintInput::Read()
	{
		PackageFileVersion Draft{};

		auto UE4 = Read<int32>();
		if (!UE4)
			return MakeError(Eh::Binary::BadRead, "Could not read UE4 version");
		Draft.UE4Version = std::move(*UE4);

		auto UE5 = Read<int32>();
		if (!UE5)
			return MakeError(Eh::Binary::BadRead, "Could not read UE5 version");
		Draft.UE5Version = std::move(*UE5);

		return Draft;
	}
#pragma endregion implementations

#pragma region Blueprint
	Result<BlueprintHeader> BlueprintInput::ReadHeader()
	{
		BlueprintHeader Draft;

		auto r_HeaderVersion = Read<uint32>();
		if (!r_HeaderVersion)
			return MakeError(Eh::Binary::BadRead, "Could not read header version");

		if (*r_HeaderVersion != 2)
			return MakeError(Eh::Binary::CheckFailed, "SBE does not support this blueprint format");
		Draft.HeaderVersion = *r_HeaderVersion;

		auto r_SaveVersion = Read<uint32>();
		if (!r_SaveVersion)
			return MakeError(Eh::Binary::BadRead, "Could not read save version");
		Draft.SaveVersion = *r_SaveVersion;

		auto r_BuildVersion = Read<uint32>();
		if (!r_BuildVersion)
			return MakeError(Eh::Binary::BadRead, "Could not read build version");
		Draft.BuildVersion = *r_BuildVersion;

		auto r_Dimensions = Read<std::array<int32, 3>>();
		if (!r_Dimensions)
			return std::unexpected(r_Dimensions.error());
		Draft.Dimensions = std::move(*r_Dimensions);

		std::print("Pointer is at: {}\n", GetPtrPos());

		auto r_Cost = Read<std::vector<CostEntry>>();
		if(!r_Cost)
			return MakeError(Eh::Binary::BadRead, std::format("Could not read cost, reason: {}", r_Cost.error().GetLogMessage()));
		Draft.Cost = std::move(*r_Cost);

		auto r_Recipes = Read<std::vector<ObjectReference>>();
		if (!r_Recipes)
			return MakeError(Eh::Binary::BadRead, std::format("Could not read recipes, reason: {}", r_Recipes.error().GetLogMessage()));
		Draft.Recipes = std::move(*r_Recipes);

		auto r_PackageFileVersion = Read<PackageFileVersion>();
		if (!r_PackageFileVersion)
			return std::unexpected(r_PackageFileVersion.error());

		return Draft;

	}
#pragma endregion read implementations
}