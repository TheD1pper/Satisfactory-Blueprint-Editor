#pragma once

#include "BasicDataTypes.hpp"

namespace Core
{
	class ActorHeader
	{
		String TypePath; // Type of actor, described in filesystem path
		String RootObject;
		String InstanceName; // Name of the actor object
		Uint32 Unknown{}; // This data is of uknkown use but crucial for not breaking the file
		Rotation3D Rotation;
		Position3D Position;
		Scale3D Scale;
		Uint32 NeedTransform{}; // Seems to be more like a boolean flag, semantics unclear
		Uint32 WasPlacedInLevel{}; // Seems to be more like a boolean flag, semantics unclear
	};
}