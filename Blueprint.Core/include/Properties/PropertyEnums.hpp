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
		uint8_t,
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
		uint32_t
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
			uint8_t,
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
			uint32_t,
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