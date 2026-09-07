module;

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
		// UE FString: int32 length, then the character payload including its null-terminator.
		// length == 0 -> empty string (no payload). length > 0 -> UTF-8. length < 0 -> UTF-16,
		// with |length| being the code-unit count (terminator included).
		String Draft;
		Draft.Encoding = StrEncoding::Utf8;
		Draft.Data = std::string{};

		auto r_Length = Read<int32>();
		if (!r_Length)
			return MakeError(Eh::Binary::BadRead, "Could not read string length");

		const int32 Length = *r_Length;

		if (Length == 0)
			return Draft;

		if (Length > 0)
		{
			std::string Data;
			Data.resize(static_cast<size_t>(Length));

			if (!ReadBytes(Data.data(), static_cast<uint64>(Length)))
				return MakeError(Eh::Binary::BadRead, "Could not read UTF-8 string data");

			if (!Data.empty() && Data.back() == '\0')
				Data.pop_back(); // Drop the serialized null-terminator

			Draft.Data = std::move(Data);
			Draft.Encoding = StrEncoding::Utf8;
		}
		else
		{
			const size_t CharCount = static_cast<size_t>(-static_cast<int64>(Length));

			std::wstring Data;
			Data.resize(CharCount);

			if (!ReadBytes(reinterpret_cast<char*>(Data.data()), static_cast<uint64>(CharCount) * sizeof(wchar_t)))
				return MakeError(Eh::Binary::BadRead, "Could not read UTF-16 string data");

			if (!Data.empty() && Data.back() == L'\0')
				Data.pop_back(); // Drop the serialized null-terminator

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

	template<>
	Result<EngineVersion> BlueprintInput::Read()
	{
		EngineVersion Draft{};

		auto r_Major = Read<uint16>();
		if (!r_Major)
			return MakeError(Eh::Binary::BadRead, "Could not read engine major version");
		Draft.Major = *r_Major;

		auto r_Minor = Read<uint16>();
		if (!r_Minor)
			return MakeError(Eh::Binary::BadRead, "Could not read engine minor version");
		Draft.Minor = *r_Minor;

		auto r_Patch = Read<uint16>();
		if (!r_Patch)
			return MakeError(Eh::Binary::BadRead, "Could not read engine patch version");
		Draft.Path = *r_Patch;

		auto r_ChangeList = Read<uint32>();
		if (!r_ChangeList)
			return MakeError(Eh::Binary::BadRead, "Could not read engine changelist");
		Draft.ChangeList = *r_ChangeList;

		auto r_Branch = Read<String>();
		if (!r_Branch)
			return MakeError(Eh::Binary::BadRead, "Could not read engine branch");
		Draft.Branch = std::move(*r_Branch);

		return Draft;
	}

	template<>
	Result<std::vector<CustomVersion>> BlueprintInput::Read()
	{
		std::vector<CustomVersion> Draft;

		auto r_Count = Read<uint32>();
		if (!r_Count)
			return MakeError(Eh::Binary::BadRead, "Could not read count of custom versions");
		Draft.resize(*r_Count);

		for (size_t i = 0; i < *r_Count; i++)
		{
			auto r_A = Read<uint32>();
			if (!r_A)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read custom version GUID.A of index: {}", i));
			Draft[i].A = *r_A;

			auto r_B = Read<uint32>();
			if (!r_B)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read custom version GUID.B of index: {}", i));
			Draft[i].B = *r_B;

			auto r_C = Read<uint32>();
			if (!r_C)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read custom version GUID.C of index: {}", i));
			Draft[i].C = *r_C;

			auto r_D = Read<uint32>();
			if (!r_D)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read custom version GUID.D of index: {}", i));
			Draft[i].D = *r_D;

			auto r_Version = Read<int32>();
			if (!r_Version)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read custom version number of index: {}", i));
			Draft[i].Version = *r_Version;
		}

		return Draft;
	}

	template<>
	Result<SaveObjectVersionData> BlueprintInput::Read()
	{
		SaveObjectVersionData Draft{};

		auto r_Version = Read<uint32>();
		if (!r_Version)
			return MakeError(Eh::Binary::BadRead, "Could not read save object version");
		Draft.Version = *r_Version;

		auto r_Package = Read<PackageFileVersion>();
		if (!r_Package)
			return std::unexpected(r_Package.error());
		Draft.Package = *r_Package;

		auto r_Licensee = Read<int32>();
		if (!r_Licensee)
			return MakeError(Eh::Binary::BadRead, "Could not read licensee version");
		Draft.Licensee = *r_Licensee;

		auto r_Engine = Read<EngineVersion>();
		if (!r_Engine)
			return std::unexpected(r_Engine.error());
		Draft.Engine = std::move(*r_Engine);

		auto r_Custom = Read<std::vector<CustomVersion>>();
		if (!r_Custom)
			return std::unexpected(r_Custom.error());
		Draft.Custom = std::move(*r_Custom);

		return Draft;
	}
#pragma endregion implementations

#pragma region Blueprint
	Result<BlueprintHeader> BlueprintInput::ReadHeader()
	{
		BlueprintHeader Draft{};

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

		auto r_Cost = Read<std::vector<CostEntry>>();
		if(!r_Cost)
			return MakeError(Eh::Binary::BadRead, std::format("Could not read cost, reason: {}", r_Cost.error().GetLogMessage()));
		Draft.Cost = std::move(*r_Cost);

		auto r_Recipes = Read<std::vector<ObjectReference>>();
		if (!r_Recipes)
			return MakeError(Eh::Binary::BadRead, std::format("Could not read recipes, reason: {}", r_Recipes.error().GetLogMessage()));
		Draft.Recipes = std::move(*r_Recipes);

		const std::streampos BodyStart = m_Input.tellg();
		auto r_Signature = Read<uint32>();
		m_Input.clear();
		m_Input.seekg(BodyStart);

		if (r_Signature && *r_Signature != static_cast<uint32>(UEPackageSignature))
		{
			auto r_SaveObjectData = Read<SaveObjectVersionData>();
			if (!r_SaveObjectData)
				return MakeError(Eh::Binary::BadRead, std::format("Could not read save object version data, reason: {}", r_SaveObjectData.error().GetLogMessage()));
			Draft.SaveObjectData = std::move(*r_SaveObjectData);
		}

		return Draft;
	}
#pragma endregion read implementations
}