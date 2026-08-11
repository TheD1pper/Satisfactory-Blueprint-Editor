module;

export module Core.Property:List;

import <vector>;
import <cstddef>;

namespace Core::Property
{
	// Defined later, in the primary module interface (Core.Property.ixx). Only an
	// incomplete type is needed here: std::vector supports incomplete element types,
	// which is exactly what lets PropertyList and Property recurse into each other
	// (Property -> Payload -> StructProperty -> Struct::Generic -> PropertyList -> Property).

	class Property;
}

export namespace Core::Property
{
	// PropertyList used to be a bare `using PropertyList = std::vector<Property>;` alias, but
	// std::vector<T>'s own special members are *conditionally* generated based on T's traits
	// (is T copy-constructible, trivially destructible, ...), and answering those questions for
	// Property means recursing all the way through Payload -> StructProperty -> Struct::Generic
	// -> PropertyList -> Property again. That circular trait computation resolves fine within
	// this module's own compilation, but doesn't survive being resumed from a different module
	// that only imports this one (MSVC re-derives it from scratch and can't close the loop).
	//
	// Wrapping the vector in an ordinary class sidesteps most of this: PropertyList's own
	// special members are explicitly *declared* here (unconditionally, not
	// template-conditionally), so a trait query on PropertyList itself can be answered from the
	// declaration alone. Their bodies are defined later, in Core.Property.ixx, once Property is
	// complete -- same reasoning as Struct::Generic/InventoryItem and Map::StructValue.
	//
	// Nothing here names Property directly (not even in a private member's declared type):
	// a PropertyList member declared while Property is still only forward-declared above
	// appears to have that incomplete Property "frozen" into it from an importing module's
	// point of view, even though within this module Property completes normally afterwards
	// and everything resolves fine internally. GetItems() hands back the underlying
	// vector<Property>* as void*; AppendProperty/PropertyListAt/PropertyListBegin/End in
	// Core.Property.ixx (declared after Property is complete) cast it back and do the real work.

	class PropertyList
	{
	private:
		void* m_Items;

	public:
		PropertyList();
		~PropertyList();
		PropertyList(const PropertyList&);
		PropertyList(PropertyList&&) noexcept;
		PropertyList& operator=(const PropertyList&);
		PropertyList& operator=(PropertyList&&) noexcept;

		size_t size() const;
		bool empty() const;

		void* GetItems();
		const void* GetItems() const;
	};
}
