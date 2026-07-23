#pragma once

#include <variant>

#include "BasicDataTypes.hpp"
#include "ComplexDataTypes.hpp"
#include "PropertyList.hpp"

namespace Core::Property::Struct
{
	class Box
	{
	public:
		Double MaxX{};
		Double MaxY{};
		Double MaxZ{};

		Double MinX{};
		Double MinY{};
		Double MinZ{};
		uint8_t IsValid{};
	};

	class FluidBox
	{
	public:
		float Value{};
	};

	class InventoryItem
	{
	public:
		String ItemName;
		uint32_t HasProperty{};
		String ItemType;
		PropertyList Properties;
	};

	class LinearColor
	{
	public:
		float Red{};
		float Green{};
		float Blue{};
		float Alpha{};	
	};

	class Quat
	{
	public:
		Double X{};
		Double Y{};
		Double Z{};
		Double W{};
	};

	class RailroadTrackPosition
	{
	public:
		ObjectReference Reference; 
		float Offset{};
		float Forward{};
	};

	class Vector
	{
	public:
		Double X{};
		Double Y{};
		Double Z{};
	};

	class DateTime
	{
	public:
		Int64 Value{};
	};

	class ClientIdentity
	{
	public:
		String UUID;
		ByteVector Unused;
	};

	struct Generic
	{
		PropertyList Value{};
	};

	using Type = std::variant<Box, FluidBox, InventoryItem, LinearColor, Quat, RailroadTrackPosition, Vector, DateTime, ClientIdentity, Generic>;
}