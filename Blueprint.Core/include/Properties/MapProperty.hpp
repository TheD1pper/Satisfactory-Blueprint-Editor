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

<<<<<<< HEAD
		std::vector<Map::Entry> Value;
=======
		Map::KeyVariant Key{};
		Map::ValueVariant Value{};
>>>>>>> 63ee77f610a7fa36879ae5f530119235f8b461ae
	};
}