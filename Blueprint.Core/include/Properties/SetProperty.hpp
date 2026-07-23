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
		std::vector<Set::Type> Value{};
		Set::Type Value{};
	};
}