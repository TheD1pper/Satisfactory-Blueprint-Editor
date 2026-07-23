#pragma once

#include <variant>

#include "BasicDataTypes.hpp"
#include "ComplexDataTypes.hpp"
#include "PropertyEnums.hpp"

namespace Core::Property
{
	class BoolProperty
	{
	public:
		uint8_t Value{};
	};

	class ByteProperty
	{
	public:
		bool HasValue{};
		uint8_t Value;
	};

	class EnumProperty
	{
	public:
		bool HasValue{};
		String Value;
	};

	class FloatProperty
	{
	public:
		float Value{};
	};

	class DoubleProperty
	{
	public:
		double Value{};
	};

	class IntProperty
	{
	public:
		int Value{};
	};

	class Int8Property
	{
	public:
		uint8_t Value{};
	};

	class Uint32Property
	{
	public:
		uint32_t Value{};
	};

	class Int64Property
	{
	public:
		int64_t Value{};
	};

	class NameProperty
	{
	public:
		String Value;
	};

	class ObjectProperty
	{
	public:
		ObjectReference Path;
	};

	class SoftObjectProperty
	{
	public:
		ObjectReference Path;
		uint32_t Value;
	};

	class StrProperty
	{
	public:
		String Value;
	};

	using Payload = std::variant<BoolProperty, ByteProperty, EnumProperty, FloatProperty,
		DoubleProperty, IntProperty, Int8Property, Uint32Property, Int64Property,
		NameProperty, ObjectProperty, SoftObjectProperty, StrProperty>;
}