module;

export module Core.Types;

import std;

export using byte = std::int8_t;
export using int8 = std::int8_t;
export using int16 = std::int16_t;
export using int32 = std::int32_t;
export using int64 = std::int64_t;
export using uint8 = std::uint8_t;
export using uint16 = std::uint16_t;
export using uint32 = std::uint32_t;
export using uint64 = std::uint64_t;
export using float32 = float;
export using float64 = double;

namespace Core
{
	export using ByteBlob = std::vector<byte>;

	export enum class StrEncoding
	{
		Utf8,
		Utf16
	};

	export enum class ObjectType
	{
		Actor,
		Object
	};

	export struct Transform3f
	{
		std::array<float, 4> Rotation;
		std::array<float, 3> Position;
		std::array<float, 3> Scale;
	};;
	
	export struct Guid
	{
		uint32 A;
		uint32 B;
		uint32 C;
		uint32 D;
	};

	export struct PackageFileVersion
	{
		int32 UE4Version;
		int32 UE5Version;
	};

	export struct String
	{
		StrEncoding Encoding;
		std::string Data;
	};

	export struct ObjectReference
	{
		String Level;
		String Path;
	};

	export struct EngineVersion
	{
		uint16 Major;
		uint16 Minor;
		uint16 Path;
		uint32 ChangeList;
		String Branch;
	}; 

	export struct CustomVersion
	{
		uint32 A;
		uint32 B;
		uint32 C;
		uint32 D;
		int32 Version;
	};
	
	export struct SaveObjectVersionData
	{
		uint32 Version;
		PackageFileVersion Package;
		int32 Licensee;
		EngineVersion Engine;
		std::vector<CustomVersion> Custom;
	};

	export struct ActorHeader
	{
		String ClassName;
		ObjectReference Reference;
		uint32 ObjectFlags;
		bool NeedTransform;
		Transform3f Transform;
		bool PlacedInLevel;
	};

	export struct ObjectHeader
	{
		String ClassName;
		ObjectReference Reference;
		uint32 ObjectFlags;
		String OuterPathName;
		bool NeedTransform;
		Transform3f Transform;
		bool PlacedInLevel;
	};

	export struct Object
	{
		ObjectType Type;
		std::variant<ActorHeader, ObjectHeader> Header;
	};

	export struct BlueprintHeader
	{
		int32 HeaderVersion;
		int32 SaveVersion;
		int32 BuildVersion;
		std::array<int32, 3> Dimensions;
		std::vector<ObjectReference> Cost;
		std::vector<ObjectReference> Recipes;
		SaveObjectVersionData SaveObjectData;
	};
	
	export struct BlueprintBody
	{
		std::vector<Object> Objects;
	};

	export struct Blueprint
	{
		BlueprintHeader Header;
		BlueprintBody Body;
	};
}