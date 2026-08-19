module;

module Core.Property;

import <string_view>;
import <array>;

import Helpers.Errors;

namespace Core::Property
{
#pragma region StrToEnum() 
	template<>
	Result<PropertyType> StrToEnum(const std::string_view& _Str)
	{
		for (const auto& Pair : PropertyArray)
		{
			if (_Str == Pair.StringType)
				return Pair.EnumType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<Array::ElementType> StrToEnum(const std::string_view& _Str)
	{
		for (const auto& Pair : Array::ElementArray)
		{
			if (_Str == Pair.StringType)
				return Pair.EnumType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<Map::KeyType> StrToEnum(const std::string_view& _Str)
	{
		for (const auto& Pair : Map::KeyArray)
		{
			if (_Str == Pair.StringType)
				return Pair.EnumType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<Map::ValueType> StrToEnum(const std::string_view& _Str)
	{
		for (const auto& Pair : Map::ValueArray)
		{
			if (_Str == Pair.StringType)
				return Pair.EnumType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<Set::ElementType> StrToEnum(const std::string_view& _Str)
	{
		for (const auto& Pair : Set::ElementArray)
		{
			if (_Str == Pair.StringType)
				return Pair.EnumType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<Struct::ElementType> StrToEnum(const std::string_view& _Str)
	{
		for (const auto& Pair : Struct::TypedArray)
		{
			if (_Str == Pair.StringType)
				return Pair.EnumType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}
#pragma endregion implementations

#pragma region EnumToStr()
	template<>
	Result<std::string_view> EnumToStr(const PropertyType& _Enum)
	{
		for (const auto& Pair : PropertyArray)
		{
			if (_Enum == Pair.EnumType)
				return Pair.StringType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<std::string_view> EnumToStr(const Array::ElementType& _Enum)
	{
		for (const auto& Pair : Array::ElementArray)
		{
			if (_Enum == Pair.EnumType)
				return Pair.StringType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<std::string_view> EnumToStr(const Map::KeyType& _Enum)
	{
		for (const auto& Pair : Map::KeyArray)
		{
			if (_Enum == Pair.EnumType)
				return Pair.StringType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<std::string_view> EnumToStr(const Map::ValueType& _Enum)
	{
		for (const auto& Pair : Map::ValueArray)
		{
			if (_Enum == Pair.EnumType)
				return Pair.StringType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<std::string_view> EnumToStr(const Set::ElementType& _Enum)
	{
		for (const auto& Pair : Set::ElementArray)
		{
			if (_Enum == Pair.EnumType)
				return Pair.StringType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}

	template<>
	Result<std::string_view> EnumToStr(const Struct::ElementType& _Enum)
	{
		for (const auto& Pair : Struct::TypedArray)
		{
			if (_Enum == Pair.EnumType)
				return Pair.StringType;
		}

		return std::unexpected(Eh::Error(Eh::Property::NotFound, "associated enum not found"));
	}
#pragma endregion implementations
}
