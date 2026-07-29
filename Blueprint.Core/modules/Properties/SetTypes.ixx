module;

#include <cstdint>
#include <variant>

export module Core.Property:SetTypes;

import Core.Data;

export namespace Core::Property::Set
{
	struct UInt32Type
	{
		uint32_t Value{};
	};

	// Per the wiki: a "StructProperty" set element is just 2 Uint64s, not a full
	// nested PropertyList (unlike ArrayProperty/MapProperty struct elements).
	struct StructType
	{
		uint64_t ValueA{};
		uint64_t ValueB{};
	};

	struct ObjectType
	{
		Core::ObjectReference Value{};
	};

	using Type = std::variant<UInt32Type, StructType, ObjectType>;
}
