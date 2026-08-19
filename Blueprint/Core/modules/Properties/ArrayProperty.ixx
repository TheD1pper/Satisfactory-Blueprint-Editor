module;

export module Core.Property:ArrayProperty;

import <vector>;

import Core.Data;
import :Enums;
import :ArrayTypes;

export namespace Core::Property
{
	struct ArrayProperty
	{
		Core::String Name{};
		Core::String OriginalTypeName{};
		Array::ElementType Type{};

		std::vector<Array::Type> Value{};
	};
}
