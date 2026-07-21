#pragma once

#include <variant>

#include "BasicDataTypes.hpp"
#include "ComplexDataTypes.hpp"
#include "PropertyList.hpp"

namespace Core::Property::Map
{
#pragma region MapKeys
	struct ObjectKey
	{
		ObjectReference Reference;
	};

	struct IntKey
	{
		Int Value{};
	};

	struct StructKey
	{
		Int Value0{};
		Int Value2{};
		Int Value3{};
	};
#pragma endregion Possible types of keys of the MapProperty

#pragma region MapValues

	struct ByteValue
	{
		Byte Value{};
	};

	struct IntValue
	{
		Int Value{};
	};

	struct Int64Value
	{
		Int64 Value{};
	};

	struct StructValue
	{
		PropertyList Property;
	};
#pragma endregion Possible types of values of the MapProperty

	using KeyVariant = std::variant<ObjectKey, IntKey, StructKey>;
	using ValueVariant = std::variant<ByteValue, IntValue, Int64Value, StructValue>;

	struct Entry
	{
		KeyVariant Key;
		ValueVariant Value{};
	};

}