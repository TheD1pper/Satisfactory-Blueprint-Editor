module;

export module Core.Property:Payloads;

import <vector>;

import Core.Data;
import :ArrayProperty;
import :MapProperty;
import :SetProperty;
import :StructProperty;

export namespace Core::Property
{
	struct BoolProperty
	{	
		uint8 Value{};
	};

	struct ByteProperty
	{
		bool HasValue{};
		uint8 Value;
	};

	struct EnumProperty
	{
		bool HasValue{};
		Core::String Value;
	};

	struct FloatProperty
	{
		float Value{};
	};

	struct DoubleProperty
	{
		double Value{};
	};

	struct IntProperty
	{
		int Value{};
	};

	struct Int8Property
	{
		uint8 Value{};
	};

	struct Uint32Property
	{
		uint32 Value{};
	};

	struct Int64Property
	{
		int64 Value{};
	};

	struct NameProperty
	{
		Core::String Value;
	};

	struct ObjectProperty
	{
		Core::ObjectReference Path;
	};

	struct SoftObjectProperty
	{
		Core::ObjectReference Path;
		uint32 Value;
	};

	struct StrProperty
	{
		Core::String Value;
	};

	struct TextProperty
	{
		uint32 Flags{};
		byte HistoryType{ -1 };
		bool IsCultureInvariant{};
		Core::String Value;
	};

	using Payload = std::variant<BoolProperty, ByteProperty, EnumProperty, FloatProperty,
		DoubleProperty, IntProperty, Int8Property, Uint32Property, Int64Property,
		NameProperty, ObjectProperty, SoftObjectProperty, StrProperty, TextProperty,
		ArrayProperty, MapProperty, SetProperty, StructProperty>;
}
