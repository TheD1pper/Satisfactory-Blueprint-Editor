module;

export module Core.Property:List;

import <vector>;

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
	using PropertyList = std::vector<Property>;
}
