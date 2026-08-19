module;

export module Core.Data;

import <vector>;
import <cstdint>;
import <string_view>;
import <string>;

export using byte = std::int8_t;
export using uint8 = std::uint8_t;
export using int16 = std::int16_t;
export using uint16 = std::uint16_t;
export using int32 = std::int32_t;
export using uint32 = std::uint32_t;
export using int64 = std::int64_t;
export using uint64 = std::uint64_t;

export namespace Core
{
	enum class Encoding 
	{
		Utf8, Utf16
	};

	struct BinaryString
	{
		Encoding Utf{};
		std::string Content;

		std::u16string Utf8ToUtf16(std::string_view _String);
		std::string Utf16ToUtf8(std::u16string_view _String);
	};

	using String = BinaryString;
	using ByteVector = std::vector<uint8>;

	struct Size3D
	{
		uint32 X{};
		uint32 Y{};
		uint32 Z{};
	};

	struct Rotation3D
	{
		float X{};
		float Y{};
		float Z{};
	};

	struct Rotation4D
	{
		float X{};
		float Y{};
		float Z{};
		float W{};
	};

	struct Position3D
	{
		float X{};
		float Y{};
		float Z{};
	};

	struct Scale3D
	{
		float X{};
		float Y{};
		float Z{};
	};

	struct FPackageFileVersion
	{
		int FileVersionUE4{};
		int FileVersionUE5{};
	};

	struct FEngineVersion
	{
		uint16 Major{};
		uint16 Minor{};
		uint16 Patch{};
		uint32 ChangeList{};
		String Branch{};
	};

	struct ObjectReference
	{
		String Level;
		String Path;
	};

	struct ContentEntry
	{
		String ClassName;
	};

	struct CostEntry
	{
		String ClassName{};
		uint32 Quantity{};
	};

	struct ActorHeader
	{
		String TypePath;
		String RootObject;
		String InstanceName;
		uint32 Unknown{};
		Rotation4D Rotation;
		Position3D Position;
		Scale3D Scale;
		bool NeedTransform{};
		bool IsPlaced{};
	};

	struct ComponentHeader
	{
		String TypePath;
		String RootObject;
		String InstanceName;
		String ParentActorName;
		uint32 Unknown;
	};

	enum class ObjectHeaderType
	{
		Actor, Component
	};

	// ObjectHeader/ActorObject/ComponentObject/Object, and the Blueprint/BlueprintBody/
	// BlueprintHeader types that assemble them, live in Core.Complex instead of here.
	// ActorObject/ComponentObject need a Core::Property::PropertyList once their properties
	// are parsed, and Core::Property already imports Core.Data (for Core::String etc.), so
	// keeping them here would make Core.Data import Core.Property right back -> a cycle.
	// Core.Complex sits downstream of both and closes that off.
	// Tip for future self: NEVER DO CYCLIC TYPES AGAIN
}
