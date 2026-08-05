module;

export module Core.Data;

import <vector>;
import <cstdint>;
import <string_view>;
import <string>;

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
	using ByteVector = std::vector<uint8_t>;

	struct Size3D
	{
		uint32_t X{};
		uint32_t Y{};
		uint32_t Z{};
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
		uint16_t Major{};
		uint16_t Minor{};
		uint16_t Patch{};
		uint32_t ChangeList{};
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
		uint32_t Quantity{};
	};

	struct ActorHeader
	{
		String TypePath;
		String RootObject;
		String InstanceName;
		uint32_t Unknown{};
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
		uint32_t Unknown;
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
