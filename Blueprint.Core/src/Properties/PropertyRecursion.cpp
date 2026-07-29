module Core.Property;

// Property is complete by this point (this is a module implementation unit, which
// implicitly imports Core.Property's primary interface), so the special members that
// StructTypes.ixx/MapTypes.ixx had to declare without a body can finally be defaulted.

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
