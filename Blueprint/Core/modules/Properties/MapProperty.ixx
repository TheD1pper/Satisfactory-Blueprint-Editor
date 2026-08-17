module;

export module Core.Property:MapProperty;

import <vector>;

import Core.Data;
import :Enums;
import :MapTypes;

export namespace Core::Property
{
	struct MapProperty
	{
		Core::String Name{};

		Map::KeyType KeyType{};
		Map::ValueType ValueType{};

		std::vector<Map::Entry> Value;
	};
}
