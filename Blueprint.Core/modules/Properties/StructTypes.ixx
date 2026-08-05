module;

export module Core.Property:StructTypes;

import <variant>;
import <cstdint>;

import Core.Data;
import :List;

export namespace Core::Property::Struct
{
	struct Box
	{
		double MaxX{};
		double MaxY{};
		double MaxZ{};

		double MinX{};
		double MinY{};
		double MinZ{};
		uint8_t IsValid{};
	};

	struct FluidBox
	{
	public:
		float Value{};
	};

	struct InventoryItem
	{
		Core::String ItemName;
		uint32_t HasProperty{};
		Core::String ItemType;
		PropertyList Properties;

		// PropertyList = std::vector<Property>, and Property is still incomplete at this
		// point (it's defined later, in the primary Core.Property module interface). A
		// vector of an incomplete type is fine on its own, but forming this struct's
		// *implicit* special members would force MSVC to instantiate vector<Property>'s
		// destructor/copy/move right here, before Property exists. Declaring them (without
		// defaulting them yet) defers that instantiation to PropertyRecursion.cpp, a module
		// implementation unit compiled after Property is complete.
		InventoryItem();
		~InventoryItem();
		InventoryItem(const InventoryItem&);
		InventoryItem(InventoryItem&&) noexcept;
		InventoryItem& operator=(const InventoryItem&);
		InventoryItem& operator=(InventoryItem&&) noexcept;
	};

	struct LinearColor
	{
		float Red{};
		float Green{};
		float Blue{};
		float Alpha{};
	};

	struct Quat
	{
		double X{};
		double Y{};
		double Z{};
		double W{};
	};

	struct RailroadTrackPosition
	{
		Core::ObjectReference Reference;
		float Offset{};
		float Forward{};
	};

	struct Vector
	{
		double X{};
		double Y{};
		double Z{};
	};

	struct DateTime
	{
		int64_t Value{};
	};

	struct ClientIdentity
	{
		Core::String UUID;
		Core::ByteVector Unused;
	};

	// StructProperty payloads whose type name isn't one of the fixed types above
	// (e.g. custom game structs like "InventoryStack") are just a plain PropertyList.
	struct Generic
	{
		PropertyList Value{};

		// See InventoryItem above for why these can't be left implicit.
		Generic();
		~Generic();
		Generic(const Generic&);
		Generic(Generic&&) noexcept;
		Generic& operator=(const Generic&);
		Generic& operator=(Generic&&) noexcept;
	};

	using Type = std::variant<Box, FluidBox, InventoryItem, LinearColor, Quat, RailroadTrackPosition, Vector, DateTime, ClientIdentity, Generic>;
}
