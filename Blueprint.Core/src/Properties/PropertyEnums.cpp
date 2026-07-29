module;

#include <string_view>
#include <unordered_map>

module Core.Property;

namespace Core::Property
{
	namespace Array
	{
		const std::unordered_map<ElementType, std::string_view> ElementMap{
			{ElementType::Vector, "Vector"},
			{ElementType::LinearColor, "LinearColor"},
			{ElementType::Quat, "Quat"},
			{ElementType::FluidBox, "FluidBox"},
			{ElementType::InventoryItem, "InventoryItem"},
			{ElementType::RailroadTrackPosition, "RailroadTrackPosition"},
			{ElementType::DateTime, "DateTime"},
			{ElementType::ClientIdentityInfo, "ClientIdentityInfo"}
		};
	}

	namespace Map
	{
		const std::unordered_map<KeyType, std::string_view> KeyMap{
			{KeyType::Object, "ObjectProperty"},
			{KeyType::Int, "IntProperty"},
			{KeyType::Struct, "StructProperty"}
		};

		const std::unordered_map<ValueType, std::string_view> ValueMap{
			{ValueType::Byte, "ByteProperty"},
			{ValueType::Int, "IntProperty"},
			{ValueType::Int64, "Int64Property"},
			{ValueType::Struct, "StructProperty"}
		};
	}

	namespace Set
	{
		const std::unordered_map<ElementType, std::string_view> ElementMap{
			{ElementType::UInt32, "Uint32Property"},
			{ElementType::Struct, "StructProperty"},
			{ElementType::Object, "ObjectProperty"}
		};
	}

	namespace Struct
	{
		const std::unordered_map<ElementType, std::string_view> TypedMap{
			{ElementType::Vector, "Vector"},
			{ElementType::LinearColor, "LinearColor"},
			{ElementType::Quat, "Quat"},
			{ElementType::FluidBox, "FluidBox"},
			{ElementType::InventoryItem, "InventoryItem"},
			{ElementType::RailroadTrackPosition, "RailroadTrackPosition"},
			{ElementType::DateTime, "DateTime"},
			{ElementType::ClientIdentityInfo, "ClientIdentityInfo"}
		};
	}
}
