#pragma once

#include "BasicDataTypes.hpp"

namespace Core
{
	class ComponentHeader
	{
	public:
		String TypePath; // Component type, described in filesystem path
		String RootObject;
		String InstanceName; // Name of a single conponent object
		Uint32 Unknown{};
		String ParentActorName; // Reference to the instance parent
	};
}