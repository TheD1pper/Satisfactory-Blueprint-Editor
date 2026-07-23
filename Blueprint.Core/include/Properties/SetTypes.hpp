#pragma once

#include <variant>

#include "BasicDataTypes.hpp"
#include "ComplexDataTypes.hpp"

namespace Core::Property::Set
{
	struct Uint32Type
	{
		uint32_t Value{};
	};

	struct StructType
	{
		uint64_t ValueA{};
		uint64_t ValueB{};
	};

	struct ObjectType
	{
		ObjectReference Value{};
	};

	using Type = std::variant<Uint32Type, StructType, ObjectType>;
}