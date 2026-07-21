#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

#include "BasicDataTypes.hpp"

namespace Core::Property
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
		Objects,
		Set,
		SoftObjects,
		Str,
		Struct,
		Text,
		Uint32
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

		const extern std::unordered_map<ElementType, std::string_view> ElementMap;
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

		const extern std::unordered_map<KeyType, std::string_view> KeyMap;
	}

	namespace Set
	{
		enum class ElementType : uint8_t
		{
			Uint32,
			Struct,
			Object
		};

		const extern std::unordered_map<ElementType, std::string_view> ElementMap;
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

		const extern std::unordered_map<ElementType, std::string_view> TypedMap;
	}
}