#pragma once

#include <vector>

#include "PropertyEnums.hpp"
#include "BasicDataTypes.hpp"
#include "ArrayTypes.hpp"

namespace Core::Property
{
	class ArrayProperty
	{
	public:
		String Name{};
		String OriginalTypeName{};
		Array::ElementType Type{};

		std::vector<Array::Type> Value{};
		Array::Type Value{};
	};
}