#pragma once

#include <vector>

#include "PropertyEnums.hpp"
#include "BasicDataTypes.hpp"
#include "SetTypes.hpp"

namespace Core::Property
{
	class SetProperty
	{
	public:
		String Name{};

		Set::ElementType Type{};
<<<<<<< HEAD
		std::vector<Set::Type> Value{};
=======
		Set::Type Value{};
>>>>>>> 63ee77f610a7fa36879ae5f530119235f8b461ae
	};
}