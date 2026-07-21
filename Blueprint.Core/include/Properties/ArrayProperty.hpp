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
<<<<<<< HEAD
		std::vector<Array::Type> Value{};
=======
		Array::Type Value{};
>>>>>>> 63ee77f610a7fa36879ae5f530119235f8b461ae
	};
}