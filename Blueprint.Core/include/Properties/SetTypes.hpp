#pragma once

#include <variant>

#include "BasicDataTypes.hpp"
#include "ComplexDataTypes.hpp"

namespace Core::Property::Set
{
	struct Uint32Type
	{
		Uint32 Value{};
	};

	struct StructType
	{
		Uint64 ValueA{};
		Uint64 ValueB{};
	};

	struct ObjectType
	{
		ObjectReference Value{};
	};

	using Type = std::variant<Uint32Type, StructType, ObjectType>;
}