#pragma once

#include <variant>

#include "BasicDataTypes.hpp"
#include "ComplexDataTypes.hpp"
#include "Property.hpp"

namespace Core::Property::Array
{
	struct ByteType
	{
		uint8_t Value{};
	};

	struct EnumType
	{
		String Value;
	};

	struct StrType
	{
		String Value;
	};

	struct InterfaceType
	{
		String Level;
		String Path;
	};

	struct ObjectType
	{
		String Level;
		String Path;
	};

	struct IntType
	{
		int Value{};
	};

	struct Int64Type
	{
		Int64 Value{};
	};

	struct FloatType
	{
		float Value{};
	};

	struct SoftObjectType
	{
		ObjectReference Reference;
		uint32_t Number{};
	};

	struct StructType
	{
		Property Value{};
	};

	using Type = std::variant<ByteType, EnumType, StrType, InterfaceType,
		ObjectType, IntType, Int64Type, FloatType, SoftObjectType, StructType>;
}