module;

export module Core.Property;

import <concepts>;
import <string_view>;
import <vector>;

import Helpers.Errors;
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
		uint32 Index{};

		Payload Value;
	};
}

// Struct::Generic/InventoryItem and Map::StructValue had to declare their special members
// without a body (see StructTypes.ixx/MapTypes.ixx) because PropertyList = vector<Property>
// needs Property to be complete first, and Property isn't complete until here. They're
// defaulted right here, in the primary interface, rather than in a private implementation
// unit: any module that imports Core.Property (Core.Complex embeds a PropertyList directly
// in ActorObject/ComponentObject) needs to see these definitions to resolve vector<Property>'s
// own special members, and definitions hidden in a module-private .cpp aren't visible to
// importers -- only what's declared in this interface (or its partitions) is.
namespace Core::Property::Struct
{
	InventoryItem::InventoryItem() = default;
	InventoryItem::~InventoryItem() = default;
	InventoryItem::InventoryItem(const InventoryItem&) = default;
	InventoryItem::InventoryItem(InventoryItem&&) noexcept = default;
	InventoryItem& InventoryItem::operator=(const InventoryItem&) = default;
	InventoryItem& InventoryItem::operator=(InventoryItem&&) noexcept = default;

	Generic::Generic() = default;
	Generic::~Generic() = default;
	Generic::Generic(const Generic&) = default;
	Generic::Generic(Generic&&) noexcept = default;
	Generic& Generic::operator=(const Generic&) = default;
	Generic& Generic::operator=(Generic&&) noexcept = default;
}

namespace Core::Property::Map
{
	StructValue::StructValue() = default;
	StructValue::~StructValue() = default;
	StructValue::StructValue(const StructValue&) = default;
	StructValue::StructValue(StructValue&&) noexcept = default;
	StructValue& StructValue::operator=(const StructValue&) = default;
	StructValue& StructValue::operator=(StructValue&&) noexcept = default;
}

namespace Core::Property
{
	// PropertyList's own members can only be *instantiated* here, inside this module: MSVC
	// isn't able to re-derive vector<Property>'s recursive completeness (Property ->> Payload
	// ->> StructProperty ->> Struct::Generic ->> PropertyList) from a different module's
	// translation unit, even once everything it depends on is visibly `=default`ed above in
	// this same interface. Declaring PropertyList's own special members explicitly (see
	// List.ixx) means external modules never need to ask "is PropertyList copy-constructible"
	// etc. via std::vector's conditional machinery -- the declarations alone answer that -- so
	// PropertyList can be used as an ordinary value type everywhere, same as Property itself.
	//
	// The real vector<Property> lives behind GetItems()'s void* on purpose (see List.ixx):
	// even a *private* `std::vector<Property> m_Items;` member declared in the :List partition,
	// while Property is still only forward-declared there, ends up with that incomplete Property
	// frozen into PropertyList's shape from an importing module's point of view. Everything that
	// actually names Property -- construction, copy, append, element access, iteration -- is
	// declared down here instead, after Property is complete.

	PropertyList::PropertyList() : m_Items(new std::vector<Property>()) {}

	PropertyList::~PropertyList()
	{
		delete static_cast<std::vector<Property>*>(m_Items);
	}

	PropertyList::PropertyList(const PropertyList& _Other) :
		m_Items(new std::vector<Property>(*static_cast<std::vector<Property>*>(_Other.m_Items)))
	{
	}

	PropertyList::PropertyList(PropertyList&& _Other) noexcept :
		m_Items(_Other.m_Items)
	{
		_Other.m_Items = nullptr;
	}

	PropertyList& PropertyList::operator=(const PropertyList& _Other)
	{
		if (this != &_Other)
		{
			delete static_cast<std::vector<Property>*>(m_Items);
			m_Items = new std::vector<Property>(*static_cast<std::vector<Property>*>(_Other.m_Items));
		}
		return *this;
	}

	PropertyList& PropertyList::operator=(PropertyList&& _Other) noexcept
	{
		if (this != &_Other)
		{
			delete static_cast<std::vector<Property>*>(m_Items);
			m_Items = _Other.m_Items;
			_Other.m_Items = nullptr;
		}
		return *this;
	}

	size_t PropertyList::size() const
	{
		auto* Items = static_cast<std::vector<Property>*>(m_Items);
		return Items ? Items->size() : 0;
	}

	bool PropertyList::empty() const
	{
		auto* Items = static_cast<std::vector<Property>*>(m_Items);
		return !Items || Items->empty();
	}

	void* PropertyList::GetItems()
	{
		return m_Items;
	}

	const void* PropertyList::GetItems() const
	{
		return m_Items;
	}
}

export namespace Core::Property
{
	void AppendProperty(PropertyList& _List, Property _Item)
	{
		static_cast<std::vector<Property>*>(_List.GetItems())->push_back(std::move(_Item));
	}

	Property& PropertyListAt(PropertyList& _List, size_t _Index)
	{
		return (*static_cast<std::vector<Property>*>(_List.GetItems()))[_Index];
	}

	const Property& PropertyListAt(const PropertyList& _List, size_t _Index)
	{
		return (*static_cast<const std::vector<Property>*>(_List.GetItems()))[_Index];
	}

	// Named `begin`/`end` (rather than e.g. PropertyListBegin) so range-based for loops over a
	// PropertyList find them via ADL, same as any other container.
	std::vector<Property>::iterator begin(PropertyList& _List)
	{
		return static_cast<std::vector<Property>*>(_List.GetItems())->begin();
	}

	std::vector<Property>::iterator end(PropertyList& _List)
	{
		return static_cast<std::vector<Property>*>(_List.GetItems())->end();
	}

	std::vector<Property>::const_iterator begin(const PropertyList& _List)
	{
		return static_cast<const std::vector<Property>*>(_List.GetItems())->begin();
	}

	std::vector<Property>::const_iterator end(const PropertyList& _List)
	{
		return static_cast<const std::vector<Property>*>(_List.GetItems())->end();
	}
}

export namespace Core::Property
{
	/// <summary>
	/// Iterates through the array looking for the associated string. When strings match returns the enumeration
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Str">- String to convert</param>
	/// <returns>std::expected with Eh::Property or one of enum types</returns>
	template<typename T> requires TypeEnums<T>
	Result<T> StrToEnum(const std::string_view& _Str);

	/// <summary>
	/// Iterates through the array looking for the associated enum. When strings match returns the string
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Enum">- Enum to convert</param>
	/// <returns>std::expected with Eh::Property or std::string_view</returns>
	template<typename T> requires TypeEnums<T>
	Result<std::string_view> EnumToStr(const T& _Enum);
}
