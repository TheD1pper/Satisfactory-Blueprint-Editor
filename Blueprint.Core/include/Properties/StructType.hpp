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
		double MaxX{};
		double MaxY{};
		double MaxZ{};

		double MinX{};
		double MinY{};
		double MinZ{};
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
		double X{};
		double Y{};
		double Z{};
		double W{};
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
		double X{};
		double Y{};
		double Z{};
	};

	class DateTime
	{
	public:
		int64_t Value{};
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