#pragma once

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
		Set::Type Value{};
	};
}