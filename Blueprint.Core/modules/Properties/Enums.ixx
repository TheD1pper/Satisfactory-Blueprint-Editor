module;

export module Core.Property:Enums;

import <cstdint>;
import <string_view>;
import <array>;
import <concepts>;

export namespace Core::Property
{
	enum class PropertyType : uint8_t
	{
		Array,
		Bool,
		Byte,
		Double,
		Enum,
		Float,
		Int,
		Int8,
		Int64,
		Map,
		Name,
		Object,
		Set,
		SoftObject,
		Str,
		Struct,
		Text,
		UInt32
	};

	namespace Array
	{
		enum class ElementType : uint8_t
		{
			Vector,
			LinearColor,
			Quat,
			Box,
			FluidBox,
			InventoryItem,
			RailroadTrackPosition,
			DateTime,
			ClientIdentityInfo,
			Generic // Every non-defined type here is generic
		};
	}

	namespace Map
	{
		enum class KeyType : uint8_t
		{
			Object,
			Int,
			Struct
		};

		enum class ValueType : uint8_t
		{
			Byte,
			Int,
			Int64,
			Struct
		};
	}

	namespace Set
	{
		enum class ElementType : uint8_t
		{
			UInt32,
			Struct,
			Object
		};
	}

	namespace Struct
	{
		enum class ElementType : uint8_t
		{
			Vector,
			LinearColor,
			Quat,
			Box,
			FluidBox,
			InventoryItem,
			RailroadTrackPosition,
			DateTime,
			ClientIdentityInfo,
			Generic // Every non-defined type here is generic
		};
	}

	template<typename T>
	concept TypeEnums =
		std::same_as<PropertyType, T> or
		std::same_as<Array::ElementType, T> or
		std::same_as<Map::KeyType, T> or
		std::same_as<Map::ValueType, T> or
		std::same_as<Set::ElementType, T> or
		std::same_as<Struct::ElementType, T>;

	template<typename _Enum>
	requires TypeEnums<_Enum>
	struct TypePair
	{
		_Enum EnumType{};
		std::string_view StringType{};
	};

	const std::array<TypePair<PropertyType>, 18> PropertyArray{
		{
			{PropertyType::Array,      "ArrayProperty"},
			{PropertyType::Bool,       "BoolProperty"},
			{PropertyType::Byte,       "ByteProperty"},
			{PropertyType::Double,     "DoubleProperty"},
			{PropertyType::Enum,       "EnumProperty"},
			{PropertyType::Float,      "FloatProperty"},
			{PropertyType::Int,        "IntProperty"},
			{PropertyType::Int8,       "Int8Property"},
			{PropertyType::Int64,      "Int64Property"},
			{PropertyType::Map,        "MapProperty"},
			{PropertyType::Name,       "NameProperty"},
			{PropertyType::Object,     "ObjectProperty"},
			{PropertyType::Set,        "SetProperty"},
			{PropertyType::SoftObject, "SoftObjectProperty"},
			{PropertyType::Str,        "StrProperty"},
			{PropertyType::Struct,     "StructProperty"},
			{PropertyType::Text,       "TextProperty"},
			{PropertyType::UInt32,     "Uint32Property"}
		}
	};

	namespace Array
	{
		const std::array<TypePair<ElementType>, 8> ElementArray{ 
			{
				{ElementType::Vector,                "Vector"},
				{ElementType::LinearColor,           "LinearColor"},
				{ElementType::Quat,                  "Quat"},
				{ElementType::FluidBox,              "FluidBox"},
				{ElementType::InventoryItem,         "InventoryItem"},
				{ElementType::RailroadTrackPosition, "RailroadTrackPosition"},
				{ElementType::DateTime,              "DateTime"},
				{ElementType::ClientIdentityInfo,    "ClientIdentityInfo"}
			} 
		};
	}

	namespace Map
	{
		const std::array<TypePair<KeyType>, 3> KeyArray{ 
			{
				{KeyType::Object, "ObjectProperty"},
				{KeyType::Int,    "IntProperty"},
				{KeyType::Struct, "StructProperty"}
			} 
		};

		const std::array<TypePair<ValueType>, 4> ValueArray{ 
			{
				{ValueType::Byte,   "ByteProperty"},
				{ValueType::Int,    "IntProperty"},
				{ValueType::Int64,  "Int64Property"},
				{ValueType::Struct, "StructProperty"}
			} 
		};
	}

	namespace Set
	{
		const std::array<TypePair<ElementType>, 3> ElementArray{ 
			{
				{ElementType::UInt32, "Uint32Property"},
				{ElementType::Struct, "StructProperty"},
				{ElementType::Object, "ObjectProperty"}
			} 
		};
	}

	namespace Struct
	{
		const std::array<TypePair<ElementType>, 8> TypedArray{ 
			{
				{ElementType::Vector,                "Vector"},
				{ElementType::LinearColor,           "LinearColor"},
				{ElementType::Quat,                  "Quat"},
				{ElementType::FluidBox,              "FluidBox"},
				{ElementType::InventoryItem,         "InventoryItem"},
				{ElementType::RailroadTrackPosition, "RailroadTrackPosition"},
				{ElementType::DateTime,              "DateTime"},
				{ElementType::ClientIdentityInfo,    "ClientIdentityInfo"}
			} 
		};
	}
}
