module;

export module Core.Complex;

import <string>;
import <vector>;
import <variant>;

import Core.Data;
import Core.Property;

export namespace Core
{
	// Everything in this module sits downstream of both Core.Data (raw field types) and
	// Core.Property (the Property/PropertyList tree). It's split out from Core.Data because
	// ActorObject/ComponentObject need a Core::Property::PropertyList, and Core::Property
	// already imports Core.Data (for Core::String/Core::ObjectReference etc.) -- so whichever
	// of the two held these object types would have to import the other right back, forming a
	// cycle. Core.Complex depends on both without either depending back on it.

	struct ObjectHeader
	{
		Core::ObjectHeaderType Type{};
		std::variant<Core::ActorHeader, Core::ComponentHeader> Payload;
	};

	struct ActorObject
	{
		Core::ObjectReference ParentReference;
		std::vector<Core::ObjectReference> Components;
		Core::Property::PropertyList Properties;
		Core::ByteVector TrallingBytes;
	};

	struct ComponentObject
	{
		Core::Property::PropertyList Properties;
		Core::ByteVector TrallingBytes;
	};

	using Object = std::variant<ActorObject, ComponentObject>;

	struct BlueprintBody
	{
		uint32 UEPackageSignature{};
		std::vector<ObjectHeader> ObjectHeaders;
		std::vector<Object> Objects;
	};

	struct BlueprintHeader
	{
		uint32 HeaderVersion{};
		uint32 SaveVersion{};
		uint32 BuildVersion{};
		Core::Size3D Size{};
		std::vector<Core::CostEntry> CostEntries;
		std::vector<Core::ContentEntry> ContentEntries;

		uint32 SaveObjectDataVersion{};
		Core::FPackageFileVersion PackageFileVersion{};
		int LicenseeVersion{};
		Core::FEngineVersion EngineVersion{};

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
