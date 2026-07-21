#pragma once

#include "PropertyEnums.hpp"
#include "BasicDataTypes.hpp"
#include "PropertyPayloads.hpp"

namespace Core::Property
{
	class Property
	{
	public:
		String Name;
		Payload Value;
		Uint32 Index{};
		PropertyType Type{};
	};
}
