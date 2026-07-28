#pragma once

#include <variant>

#include "BasicDataTypes.hpp"

namespace Core
{
	struct ActorHeader
	{
		String TypePath;
		String RootObject;
		String InstanceName;
		uint32_t Unknown{};
		Rotation4D Rotation;
		Position3D Position;
		Scale3D Scale;
		bool NeedTransform{};
		bool IsPlaced{};
	};

	struct ComponentHeader
	{
		String TypePath;
		String RootObject;
		String InstanceName;
		String ParentActorName;
		uint32_t Unknowk;
	};

	enum class ObjectHeaderType
	{
		Actor, Component
	};

	struct ObjectHeader
	{
		ObjectHeaderType Type{};
		std::variant<ActorHeader, ComponentHeader> Payload;
	};

	struct ActorObject
	{
		ObjectReference ParentReference;
		std::vector<ObjectReference> Components;
		//PropertyList Properties;
		ByteVector TrallingBytes;
	};

	struct ComponentObject
	{
		//PropertyList Properties;
		ByteVector TrallingBytes;
	};

	using Object = std::variant<ActorObject, ActorHeader>;
}