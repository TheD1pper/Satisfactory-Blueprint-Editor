#pragma once

#include <vector>

#include "PropertyEnums.hpp"
#include "BasicDataTypes.hpp"
#include "MapTypes.hpp"

namespace Core::Property
{
	class MapProperty
	{
	public:
		String Name{};

		Map::KeyType KeyType{};
		Map::ValueType ValueType{};

		std::vector<Map::Entry> Value;
	};
}