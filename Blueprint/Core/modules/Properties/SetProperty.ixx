module;

export module Core.Property:SetProperty;

import <vector>;

import Core.Data;
import :Enums;
import :SetTypes;

export namespace Core::Property
{
	struct SetProperty
	{
		Core::String Name{};

		Set::ElementType Type{};
		std::vector<Set::Type> Value{};
	};
}
