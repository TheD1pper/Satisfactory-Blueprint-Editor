#pragma once

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

		Map::KeyVariants Key{};
		Map::ValueVariants Value{};
	};
}