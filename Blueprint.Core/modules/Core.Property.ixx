module;

#include <cstdint>

export module Core.Property;

import Core.Data;

export import :Enums;
export import :List;
export import :StructTypes;
export import :StructProperty;
export import :ArrayTypes;
export import :ArrayProperty;
export import :MapTypes;
export import :MapProperty;
export import :SetTypes;
export import :SetProperty;
export import :Payloads;

export namespace Core::Property
{
	// The recursive core of the save-file property tree:
	//   Property     ->> Payload  (may hold a StructProperty/ArrayProperty/MapProperty/SetProperty)
	//   StructProperty ->> PropertyList (Struct::Generic / InventoryItem hold a nested PropertyList)
	//   PropertyList = std::vector<Property>, closing the cycle back to Property.
	class Property
	{
	public:
		Core::String Name;
		PropertyType Type{};
		uint32_t Index{};

		Payload Value;
	};
}
