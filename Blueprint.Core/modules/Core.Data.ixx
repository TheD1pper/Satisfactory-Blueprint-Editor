module;

#include <string>
#include <string_view>
#include <cstdint>
#include <vector>
#include <variant>

export module Core.Data;

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
		uint32_t Unknowk;
	};

	enum class ObjectHeaderType
	{
		Actor, Component
	};

	struct ObjectHeader
	{
		ObjectHeaderType Type{};
		std::variant<ActorHeader, ComponentHeader> Payload;
	};

	struct ActorObject
	{
		ObjectReference ParentReference;
		std::vector<ObjectReference> Components;
		ByteVector TrallingBytes;
	};

	struct ComponentObject
	{
		ByteVector TrallingBytes;
	};

	using Object = std::variant<ActorObject, ComponentObject>;

	struct BlueprintBody
	{
		uint32_t UEPackageSignature{};
		uint64_t CompressedSize{};
		uint64_t UncompressedSize{};

		// ===== Decompressed variables =====

		uint32_t UncompressedBodySize{};
		uint32_t ObjectHeadersSize{};
		std::vector<ObjectHeader> ObjectHeader;
		uint32_t ObjectsSize;
		std::vector<Object> Objects;
	};

	struct BlueprintHeader
	{
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
		std::string Name;
		BlueprintHeader Header;
		BlueprintBody Body;

	public:
		Blueprint() = default; // Default constructor
		inline Blueprint(BlueprintHeader&& _Header, BlueprintBody&& _Body, std::string&& _Name) :
			Name(std::move(_Name)),
			Header(std::move(_Header)),
			Body(std::move(_Body)) {
		}
	};
}
