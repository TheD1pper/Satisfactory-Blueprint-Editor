module;

#include <cstdint>
#include <variant>

export module Core.Property:ArrayTypes;

import Core.Data;
import :StructTypes;

export namespace Core::Property::Array
{
	struct ByteType
	{
		uint8_t Value{};
	};

	struct EnumType
	{
		Core::String Value;
	};

	struct StrType
	{
		Core::String Value;
	};

	struct InterfaceType
	{
		Core::String Level;
		Core::String Path;
	};

	struct ObjectType
	{
		Core::String Level;
		Core::String Path;
	};

	struct IntType
	{
		int Value{};
	};

	struct Int64Type
	{
		int64_t Value{};
	};

	struct FloatType
	{
		float Value{};
	};

	struct SoftObjectType
	{
		Core::ObjectReference Reference;
		uint32_t Number{};
	};

	// Per element type name (see StructTypes.ixx); a generic array element is a
	// PropertyList by way of Struct::Generic.
	struct StructType
	{
		Core::String ElementTypeName;
		Struct::Type Value{};
	};

	using Type = std::variant<ByteType, EnumType, StrType, InterfaceType,
		ObjectType, IntType, Int64Type, FloatType, SoftObjectType, StructType>;
}
