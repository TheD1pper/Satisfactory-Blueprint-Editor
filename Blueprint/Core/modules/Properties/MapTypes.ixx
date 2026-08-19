module;

export module Core.Property:MapTypes;

import <variant>;

import Core.Data;
import :List;

export namespace Core::Property::Map
{
	struct ObjectKey
	{
		Core::ObjectReference Reference;
	};

	struct IntKey
	{
		int Value{};
	};

	struct StructKey
	{
		int Value0{};
		int Value2{};
		int Value3{};
	};

	struct ByteValue
	{
		uint8 Value{};
	};

	struct IntValue
	{
		int Value{};
	};

	struct Int64Value
	{
		int64 Value{};
	};

	// A "StructProperty" map value is a whole nested PropertyList (see PropertyList
	// section of the wiki: "a list of properties per key-value pair").
	struct StructValue
	{
		PropertyList Value;

		// Same reasoning as Struct::Generic/Struct::InventoryItem in StructTypes.ixx:
		// Property is still incomplete here, so these must be defaulted later, in
		// PropertyRecursion.cpp, once Core.Property's primary interface has defined it.
		StructValue();
		~StructValue();
		StructValue(const StructValue&);
		StructValue(StructValue&&) noexcept;
		StructValue& operator=(const StructValue&);
		StructValue& operator=(StructValue&&) noexcept;
	};

	using KeyVariant = std::variant<ObjectKey, IntKey, StructKey>;
	using ValueVariant = std::variant<ByteValue, IntValue, Int64Value, StructValue>;

	struct Entry
	{
		KeyVariant Key;
		ValueVariant Value{};
	};
}
