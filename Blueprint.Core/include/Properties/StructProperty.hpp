#pragma once

#include <optional>

#include "PropertyList.hpp"
#include "PropertyEnums.hpp"
#include "StructType.hpp"

namespace Core::Property
{
	class StructProperty
	{
	public:
		String Name;
		std::optional<String> OriginalType{std::nullopt};

		Struct::Type Value{};

		bool IsTypeGeneric{};
		Struct::ElementType Type{};
	};
}