#pragma once

#include "PropertyEnums.hpp"
#include "BasicDataTypes.hpp"
#include "ArrayTypes.hpp"

namespace Core::Property
{
	class ArrayProperty
	{
	public:
		String Name{};

		Array::ElementType Type{};
		Array::Type Value{};
	};
}