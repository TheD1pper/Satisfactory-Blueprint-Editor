module;

#include <cstdint>

export module Core.Property;

import <concepts>;
import <string_view>;

import Helpers.Errors;
import Core.Data;

export import :Enums;
export import :List;
export import :StructTypes;
export import :StructProperty;
export import :ArrayTypes;
export import :ArrayProperty;
export import :MapTypes;
export import :MapProperty;
export import :SetTypes;
export import :SetProperty;
export import :Payloads;

export namespace Core::Property
{
	// The recursive core of the save-file property tree:
	//   Property     ->> Payload  (may hold a StructProperty/ArrayProperty/MapProperty/SetProperty)
	//   StructProperty ->> PropertyList (Struct::Generic / InventoryItem hold a nested PropertyList)
	//   PropertyList = std::vector<Property>, closing the cycle back to Property.

	class Property
	{
	public:
		Core::String Name;
		PropertyType Type{};
		uint32_t Index{};

		Payload Value;
	};

	/// <summary>
	/// Iterates through the array looking for the associated string. When strings match returns the enumeration
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Str">- String to convert</param>
	/// <returns>std::expected with Eh::Property or one of enum types</returns>
	template<typename T> requires TypeEnums<T>
	Result<T> StrToEnum(const std::string_view& _Str);

	/// <summary>
	/// Iterates through the array looking for the associated enum. When strings match returns the string
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Enum">- Enum to convert</param>
	/// <returns>std::expected with Eh::Property or std::string_view</returns>
	template<typename T> requires TypeEnums<T>
	Result<std::string_view> EnumToStr(const T& _Enum);
}
