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
		Byte IsValid{};
	};

	class FluidBox
	{
	public:
		Float Value{};
	};

	class LinearColor
	{
	public:
		Float Red{};
		Float Green{};
		Float Blue{};
		Float Alpha{};	
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
		Float Offset{};
		Float Forward{};
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

	class ClientIndentity
	{
	public:
		String UUID;
		ByteVector Unused;
	};

	struct Generic
	{
		PropertyList Value{};
	};

	using Type = std::variant<Box, FluidBox, LinearColor, Quat, RailroadTrackPosition, Vector, DateTime, ClientIndentity>;
}