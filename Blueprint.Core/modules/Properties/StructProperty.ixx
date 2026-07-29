module;

#include <optional>

export module Core.Property:StructProperty;

import Core.Data;
import :Enums;
import :StructTypes;

export namespace Core::Property
{
	struct StructProperty
	{
		Core::String Name;
		std::optional<Core::String> OriginalType{ std::nullopt };

		Struct::Type Value{};

		bool IsTypeGeneric{};
		Struct::ElementType Type{};
	};
}
