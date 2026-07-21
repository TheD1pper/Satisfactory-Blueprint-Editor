#pragma once

#include <variant>

#include "BasicDataTypes.hpp"
#include "ComplexDataTypes.hpp"
#include "Property.hpp"

namespace Core::Property::Array
{
	struct ByteType
	{
		Byte Value{};
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
		Int Value{};
	};

	struct Int64Type
	{
		Int64 Value{};
	};

	struct FloatType
	{
		Float Value{};
	};

	struct SoftObjectType
	{
		ObjectReference Reference;
		Uint32 Number{};
	};

	struct StructType
	{
		Property Value{};
	};

	using Type = std::variant<ByteType, EnumType, StrType, InterfaceType,
		ObjectType, IntType, Int64Type, FloatType, SoftObjectType, StructType>;
}