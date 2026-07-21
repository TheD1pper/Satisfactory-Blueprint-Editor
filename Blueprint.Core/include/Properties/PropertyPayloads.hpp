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
		Byte Value{};
	};

	class ByteProperty
	{
	public:
		bool HasValue{};
		Byte Value;
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
		Float Value{};
	};

	class DoubleProperty
	{
	public:
		Double Value{};
	};

	class IntProperty
	{
	public:
		Int Value{};
	};

	class Int8Property
	{
	public:
		Byte Value{};
	};

	class Uint32Property
	{
	public:
		Uint32 Value{};
	};

	class Int64Property
	{
	public:
		Int64 Value{};
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
		Uint32 Value;
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