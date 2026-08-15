module;

#include <cstdint>
#include <expected>
#include <utility>

module Parser.BinaryIO;

import <cstdint>;
import <string>;

import Helpers.Errors;
import Core.Data;
import Core.Property;

using namespace Core::Property;

// Wire format per the Official Satisfactory Wiki (satisfactory.wiki.gg/wiki/Save_files,
// "Save file format" -> "Composite data types" -> Property/PropertyList):
//
//   Name: String                         -- "None" terminates a PropertyList, nothing follows
//   TypeName: String                     -- e.g. "IntProperty", mapped via StrToEnum<PropertyType>
//   PayloadSize: Uint32                  -- byte size of the payload below
//   Index: Uint32                        -- "seems to always be zero"
//   [type-specific additional headers, read inline in Read<Property>()'s switch below]
//   PadByte: Uint8                       -- always zero
//   -- payload, exactly PayloadSize bytes --
//
// Every property is wrapped with a "read exactly PayloadSize bytes" resync in
// Read<Property>(): if a payload reader under/over-reads relative to PayloadSize (e.g.
// because a sub-format here is wrong or a case isn't modeled yet), the stream is snapped
// back into alignment before the next property is read, so one bad property doesn't
// corrupt everything after it.

namespace Parser
{
	// Declared up front (defined at the bottom, in the Property / PropertyList region): the
	// Struct::Generic/InventoryItem and Map::StructValue payload readers below need to recurse
	// into a nested PropertyList before this file reaches its own definition, and an explicit
	// specialization must be visible before its first use in the same translation unit.

	template<> Result<Property> InputBlueprint::Read();
	template<> Result<PropertyList> InputBlueprint::Read();

#pragma region Scalar payloads

	template<>
	Result<BoolProperty> InputBlueprint::Read()
	{
		auto r_Value = Read<uint8_t>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read bool property value"));
		return BoolProperty{ *r_Value };
	}

	template<>
	Result<FloatProperty> InputBlueprint::Read()
	{
		auto r_Value = Read<float>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read float property value"));
		return FloatProperty{ *r_Value };
	}

	template<>
	Result<DoubleProperty> InputBlueprint::Read()
	{
		auto r_Value = Read<double>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read double property value"));
		return DoubleProperty{ *r_Value };
	}

	template<>
	Result<IntProperty> InputBlueprint::Read()
	{
		auto r_Value = Read<int>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read int property value"));
		return IntProperty{ *r_Value };
	}

	template<>
	Result<Int8Property> InputBlueprint::Read()
	{
		auto r_Value = Read<uint8_t>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read int8 property value"));
		return Int8Property{ *r_Value };
	}

	template<>
	Result<Uint32Property> InputBlueprint::Read()
	{
		auto r_Value = Read<uint32_t>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read uint32 property value"));
		return Uint32Property{ *r_Value };
	}

	template<>
	Result<Int64Property> InputBlueprint::Read()
	{
		auto r_Value = Read<int64_t>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read int64 property value"));
		return Int64Property{ *r_Value };
	}

	template<>
	Result<NameProperty> InputBlueprint::Read()
	{
		auto r_Value = Read<Core::String>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read name property value"));
		return NameProperty{ *r_Value };
	}

	template<>
	Result<StrProperty> InputBlueprint::Read()
	{
		auto r_Value = Read<Core::String>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read str property value"));
		return StrProperty{ *r_Value };
	}

	template<>
	Result<ObjectProperty> InputBlueprint::Read()
	{
		auto r_Value = Read<Core::ObjectReference>();
		if (!r_Value)
			return std::unexpected(r_Value.error());
		return ObjectProperty{ *r_Value };
	}

	template<>
	Result<SoftObjectProperty> InputBlueprint::Read()
	{
		auto r_Path = Read<Core::ObjectReference>();
		if (!r_Path)
			return std::unexpected(r_Path.error());

		auto r_Value = Read<uint32_t>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read soft object property extra value"));

		return SoftObjectProperty{ *r_Path, *r_Value };
	}

	// FText, "Base" history only (namespace/key skipped when culture-invariant, source string
	// always present). Any other history type (NamedFormat, AsNumber, StringTableEntry, ...)
	// is reported as an error rather than guessed at.

	template<>
	Result<TextProperty> InputBlueprint::Read()
	{
		// Per the wiki (satisfactory.wiki.gg/wiki/Save_files, TextProperty): flat and
		// unconditional, no history-type branching and no separate namespace/key fields --
		// those exist in stock Unreal's FText but Satisfactory's serializer doesn't use them.
		// History type "seems to always be -1"; IsCultureInvariant is a Uint32, not a byte.

		TextProperty Draft;

		auto r_Flags = Read<uint32_t>();
		if (!r_Flags)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read text property flags"));
		Draft.Flags = *r_Flags;

		auto r_History = Read<uint8_t>();
		if (!r_History)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read text property history type"));
		Draft.HistoryType = static_cast<int8_t>(*r_History);

		auto r_CultureInvariant = Read<uint32_t>();
		if (!r_CultureInvariant)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read text property culture-invariant flag"));
		Draft.IsCultureInvariant = (*r_CultureInvariant != 0);

		auto r_Source = Read<Core::String>();
		if (!r_Source)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read text property source string"));
		Draft.Value = *r_Source;

		return Draft;
	}

#pragma endregion Scalar payloads

#pragma region Byte / Enum payloads

	// ByteProperty carries an EnumName tag field: "None" means the value is a raw byte,
	// anything else means the value is serialized as the chosen entry's name instead. This
	// struct only has room for a numeric Value, so the named-enum form is consumed correctly
	// (to keep the stream aligned) but the name itself isn't kept.

	static Result<ByteProperty> ReadBytePayload(InputBlueprint& _Input, const Core::String& _EnumName)
	{
		ByteProperty Draft;

		if (_EnumName.Content.empty() or _EnumName.Content == "None")
		{
			Draft.HasValue = false;
			auto r_Value = _Input.Read<uint8_t>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read byte property value"));
			Draft.Value = *r_Value;

			return Draft;
		}

		Draft.HasValue = true;
		auto r_Value = _Input.Read<Core::String>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read byte property enum value"));
		Draft.Value = 0; // Named-enum form isn't representable here; see comment above

		return Draft;
	}

	static Result<EnumProperty> ReadEnumPayload(InputBlueprint& _Input)
	{
		EnumProperty Draft;

		auto r_Value = _Input.Read<Core::String>();
		if (!r_Value)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read enum property value"));
		Draft.HasValue = true;
		Draft.Value = *r_Value;

		return Draft;
	}

#pragma endregion Byte / Enum payloads

#pragma region Array payload

	// Every array inner type except StructProperty is a densely packed, un-tagged element.

	static Result<Array::Type> ReadArrayElement(InputBlueprint& _Input, const std::string& _InnerTypeName)
	{
		if (_InnerTypeName == "ByteProperty")
		{
			auto r_Value = _Input.Read<uint8_t>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array byte element"));
			return Array::Type{ Array::ByteType{ *r_Value } };
		}
		if (_InnerTypeName == "EnumProperty")
		{
			auto r_Value = _Input.Read<Core::String>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array enum element"));
			return Array::Type{ Array::EnumType{ *r_Value } };
		}
		if (_InnerTypeName == "StrProperty")
		{
			auto r_Value = _Input.Read<Core::String>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array string element"));
			return Array::Type{ Array::StrType{ *r_Value } };
		}
		if (_InnerTypeName == "InterfaceProperty")
		{
			auto r_Value = _Input.Read<Core::ObjectReference>();
			if (!r_Value)
				return std::unexpected(r_Value.error());
			return Array::Type{ Array::InterfaceType{ r_Value->Level, r_Value->Path } };
		}
		if (_InnerTypeName == "ObjectProperty")
		{
			auto r_Value = _Input.Read<Core::ObjectReference>();
			if (!r_Value)
				return std::unexpected(r_Value.error());
			return Array::Type{ Array::ObjectType{ r_Value->Level, r_Value->Path } };
		}
		if (_InnerTypeName == "IntProperty")
		{
			auto r_Value = _Input.Read<int>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array int element"));
			return Array::Type{ Array::IntType{ *r_Value } };
		}
		if (_InnerTypeName == "Int64Property")
		{
			auto r_Value = _Input.Read<int64_t>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array int64 element"));
			return Array::Type{ Array::Int64Type{ *r_Value } };
		}
		if (_InnerTypeName == "FloatProperty")
		{
			auto r_Value = _Input.Read<float>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array float element"));
			return Array::Type{ Array::FloatType{ *r_Value } };
		}
		if (_InnerTypeName == "SoftObjectProperty")
		{
			auto r_Ref = _Input.Read<Core::ObjectReference>();
			if (!r_Ref)
				return std::unexpected(r_Ref.error());
			auto r_Num = _Input.Read<uint32_t>();
			if (!r_Num)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array soft object element number"));
			return Array::Type{ Array::SoftObjectType{ *r_Ref, *r_Num } };
		}

		return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Unsupported array element type: " + _InnerTypeName));
	}

	static Result<Struct::Type> ReadStructBody(InputBlueprint& _Input, Struct::ElementType _Type, uint64_t _PayloadEnd);

	static Result<ArrayProperty> ReadArrayPayload(InputBlueprint& _Input, const Core::String& _Name, const Core::String& _InnerTypeName)
	{
		ArrayProperty Draft;
		Draft.Name = _Name;
		Draft.OriginalTypeName = _InnerTypeName;
		Draft.Type = Array::ElementType::Generic; // only meaningful when the inner type is a struct

		auto r_Count = _Input.Read<uint32_t>();
		if (!r_Count)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array element count"));

		Draft.Value.reserve(*r_Count);

		if (_InnerTypeName.Content == "StructProperty")
		{
			// Arrays of structs carry one extra nested tag describing every element (per the
			// wiki's "StructProperty (nested in ArrayProperty)" table): Name (always matches
			// the ArrayProperty's own name), TypeName ("StructProperty"), PayloadSize (combined
			// size of all values), Padding (Uint32, always zero), ElementType, a 16-byte UUID
			// (always zero), PadByte -- then Count raw struct bodies with no further per-element
			// tag.
			auto r_Name = _Input.Read<Core::String>();
			if (!r_Name)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array struct-tag name"));

			auto r_TypeName = _Input.Read<Core::String>();
			if (!r_TypeName)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array struct-tag type name"));

			auto r_PayloadSize = _Input.Read<uint32_t>();
			if (!r_PayloadSize)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array struct-tag payload size"));

			auto r_Padding = _Input.Read<uint32_t>();
			if (!r_Padding)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array struct-tag padding"));

			auto r_StructTypeName = _Input.Read<Core::String>();
			if (!r_StructTypeName)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array struct-tag struct name"));

			_Input.SkipBytes(16); // UUID, always zero

			auto r_PadByte = _Input.Read<uint8_t>();
			if (!r_PadByte)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read array struct-tag pad byte"));

			auto r_ElementType = StrToEnum<Array::ElementType>(r_StructTypeName->Content);
			Draft.Type = r_ElementType ? *r_ElementType : Array::ElementType::Generic;

			// Array::ElementType and Struct::ElementType enumerate the exact same struct type
			// names in the exact same order (see Enums.ixx) -- Array::ElementType only exists
			// as a separate type because StrToEnum<T> is keyed per distinct enum type.

			auto StructType = static_cast<Struct::ElementType>(Draft.Type);

			for (uint32_t i = 0; i < *r_Count; i++)
			{
				// Fixed struct types have a statically known size; Generic is self-terminating
				// via its nested PropertyList's "None" sentinel, so no end offset is needed here.

				auto r_Body = ReadStructBody(_Input, StructType, 0);
				if (!r_Body)
					return std::unexpected(r_Body.error());
				Draft.Value.push_back(Array::StructType{ *r_StructTypeName, std::move(*r_Body) });
			}

			return Draft;
		}

		for (uint32_t i = 0; i < *r_Count; i++)
		{
			auto r_Element = ReadArrayElement(_Input, _InnerTypeName.Content);
			if (!r_Element)
				return std::unexpected(r_Element.error());
			Draft.Value.push_back(std::move(*r_Element));
		}

		return Draft;
	}

#pragma endregion Array payload

#pragma region Map payload

	static Result<Map::KeyVariant> ReadMapKey(InputBlueprint& _Input, Map::KeyType _Type)
	{
		switch (_Type)
		{
		case Map::KeyType::Object:
		{
			auto r_Object = _Input.Read<Core::ObjectReference>();
			if (!r_Object)
				return std::unexpected(r_Object.error());
			return Map::KeyVariant{ Map::ObjectKey{ *r_Object } };
		}
		case Map::KeyType::Int:
		{
			auto r_Int = _Input.Read<int>();
			if (!r_Int)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map int key"));
			return Map::KeyVariant{ Map::IntKey{ *r_Int } };
		}
		case Map::KeyType::Struct:
		{
			// Low confidence: inferred purely from the StructKey field names (Value0/Value2/
			// Value3), not a confirmed reference. Rare in practice (most Satisfactory maps key
			// on ObjectProperty) -- flag if this turns out wrong.
			Map::StructKey Key;
			auto r_Value0 = _Input.Read<int>();
			if (!r_Value0)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map struct key"));
			Key.Value0 = *r_Value0;

			auto r_Value2 = _Input.Read<int>();
			if (!r_Value2)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map struct key"));
			Key.Value2 = *r_Value2;

			auto r_Value3 = _Input.Read<int>();
			if (!r_Value3)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map struct key"));
			Key.Value3 = *r_Value3;

			return Map::KeyVariant{ Key };
		}
		}

		return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Unknown map key type"));
	}

	static Result<Map::ValueVariant> ReadMapEntryValue(InputBlueprint& _Input, Map::ValueType _Type)
	{
		switch (_Type)
		{
		case Map::ValueType::Byte:
		{
			auto r_Byte = _Input.Read<uint8_t>();
			if (!r_Byte)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map byte value"));
			return Map::ValueVariant{ Map::ByteValue{ *r_Byte } };
		}
		case Map::ValueType::Int:
		{
			auto r_Int = _Input.Read<int>();
			if (!r_Int)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map int value"));
			return Map::ValueVariant{ Map::IntValue{ *r_Int } };
		}
		case Map::ValueType::Int64:
		{
			auto r_Int64 = _Input.Read<int64_t>();
			if (!r_Int64)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map int64 value"));
			return Map::ValueVariant{ Map::Int64Value{ *r_Int64 } };
		}
		case Map::ValueType::Struct:
		{
			// A StructProperty map value is a whole nested PropertyList with no type-name/guid
			// wrapper (unlike ArrayProperty's struct elements) -- see MapTypes.ixx.
			Map::StructValue Value;
			auto r_PropertyList = _Input.Read<PropertyList>();
			if (!r_PropertyList)
				return std::unexpected(r_PropertyList.error());
			Value.Value = std::move(*r_PropertyList);
			return Map::ValueVariant{ Value };
		}
		}

		return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Unknown map value type"));
	}

	static Result<MapProperty> ReadMapPayload(InputBlueprint& _Input, const Core::String& _Name, const Core::String& _KeyTypeName, const Core::String& _ValueTypeName)
	{
		MapProperty Draft;
		Draft.Name = _Name;

		auto r_KeyType = StrToEnum<Map::KeyType>(_KeyTypeName.Content);
		if (!r_KeyType)
			return std::unexpected(r_KeyType.error());
		Draft.KeyType = *r_KeyType;

		auto r_ValueType = StrToEnum<Map::ValueType>(_ValueTypeName.Content);
		if (!r_ValueType)
			return std::unexpected(r_ValueType.error());
		Draft.ValueType = *r_ValueType;

		_Input.SkipBytes(4); // Unused "removed items" count, always 0 in practice

		auto r_Count = _Input.Read<uint32_t>();
		if (!r_Count)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map entry count"));

		Draft.Value.reserve(*r_Count);

		for (uint32_t i = 0; i < *r_Count; i++)
		{
			Map::Entry Entry;

			auto r_Key = ReadMapKey(_Input, Draft.KeyType);
			if (!r_Key)
				return std::unexpected(r_Key.error());
			Entry.Key = std::move(*r_Key);

			auto r_Value = ReadMapEntryValue(_Input, Draft.ValueType);
			if (!r_Value)
				return std::unexpected(r_Value.error());
			Entry.Value = std::move(*r_Value);
			Draft.Value.push_back(std::move(Entry));
		}

		return Draft;
	}

#pragma endregion Map payload

#pragma region Set payload

	static Result<Set::Type> ReadSetElement(InputBlueprint& _Input, Set::ElementType _Type)
	{
		switch (_Type)
		{
		case Set::ElementType::UInt32:
		{
			auto r_UInt32 = _Input.Read<uint32_t>();
			if (!r_UInt32)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read set uint32 element"));

			return Set::Type{ Set::UInt32Type{ *r_UInt32 } };
		}
		case Set::ElementType::Struct:
		{
			// Per the wiki: a "StructProperty" set element is just 2 uint64s, not a full nested
			// PropertyList -- see SetTypes.ixx.
			auto r_ValueA = _Input.Read<uint64_t>();
			if (!r_ValueA)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read set struct element"));

			auto r_ValueB = _Input.Read<uint64_t>();
			if (!r_ValueB)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read set struct element"));

			return Set::Type{ Set::StructType{ *r_ValueA, *r_ValueB } };
		}
		case Set::ElementType::Object:
		{
			auto r_Object = _Input.Read<Core::ObjectReference>();
			if (!r_Object)
				return std::unexpected(r_Object.error());

			return Set::Type{ Set::ObjectType{ *r_Object } };
		}
		}

		return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Unknown set element type"));
	}

	static Result<SetProperty> ReadSetPayload(InputBlueprint& _Input, const Core::String& _Name, const Core::String& _ElementTypeName)
	{
		SetProperty Draft;
		Draft.Name = _Name;

		auto r_Type = StrToEnum<Set::ElementType>(_ElementTypeName.Content);
		if (!r_Type)
			return std::unexpected(r_Type.error());
		Draft.Type = *r_Type;

		_Input.SkipBytes(4); // Unused "removed items" count, always 0 in practice

		auto r_Count = _Input.Read<uint32_t>();
		if (!r_Count)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read set element count"));

		Draft.Value.reserve(*r_Count);

		for (uint32_t i = 0; i < *r_Count; i++)
		{
			auto r_Element = ReadSetElement(_Input, Draft.Type);
			if (!r_Element)
				return std::unexpected(r_Element.error());
			Draft.Value.push_back(std::move(*r_Element));
		}

		return Draft;
	}

#pragma endregion Set payload

#pragma region Struct payload

	// Fixed-layout struct bodies are read in exactly the field declaration order of their
	// StructTypes.ixx definition, since that's the clearest record of the wire order that was
	// already reverse-engineered when those structs were written (e.g. Box is Max* then Min*
	// then IsValid, not the more conventional Min-then-Max order).
	static Result<Struct::Type> ReadStructBody(InputBlueprint& _Input, Struct::ElementType _Type, uint64_t _PayloadEnd)
	{
		switch (_Type)
		{
		case Struct::ElementType::Vector:
		{
			Struct::Vector Draft;
			auto r_X = _Input.Read<double>();
			if (!r_X)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Vector.X"));
			Draft.X = *r_X;

			auto r_Y = _Input.Read<double>();
			if (!r_Y)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Vector.Y"));
			Draft.Y = *r_Y;

			auto r_Z = _Input.Read<double>();
			if (!r_Z)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Vector.Z"));
			Draft.Z = *r_Z;

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::LinearColor:
		{
			Struct::LinearColor Draft;
			auto r_Red = _Input.Read<float>();
			if (!r_Red) 
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read LinearColor.Red"));
			Draft.Red = *r_Red;

			auto r_Green = _Input.Read<float>();
			if (!r_Green)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read LinearColor.Green"));
			Draft.Green = *r_Green;

			auto r_Blue = _Input.Read<float>();
			if (!r_Blue)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read LinearColor.Blue"));
			Draft.Blue = *r_Blue;

			auto r_Alpha = _Input.Read<float>();
			if (!r_Alpha)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read LinearColor.Alpha"));
			Draft.Alpha = *r_Alpha;

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::Quat:
		{
			Struct::Quat Draft;
			auto r_X = _Input.Read<double>();
			if (!r_X)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Quat.X"));
			Draft.X = *r_X;

			auto r_Y = _Input.Read<double>();
			if (!r_Y)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Quat.Y"));
			Draft.Y = *r_Y;

			auto r_Z = _Input.Read<double>();
			if (!r_Z)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Quat.Z"));
			Draft.Z = *r_Z;

			auto r_W = _Input.Read<double>();
			if (!r_W)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Quat.W"));
			Draft.W = *r_W;

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::Box:
		{
			Struct::Box Draft;
			auto r_MaxX = _Input.Read<double>();
			if (!r_MaxX)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Box.MaxX"));
			Draft.MaxX = *r_MaxX;

			auto r_MaxY = _Input.Read<double>();
			if (!r_MaxY)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Box.MaxY"));
			Draft.MaxY = *r_MaxY;

			auto r_MaxZ = _Input.Read<double>();
			if (!r_MaxZ)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Box.MaxZ"));
			Draft.MaxZ = *r_MaxZ;

			auto r_MinX = _Input.Read<double>();
			if (!r_MinX)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Box.MinX"));
			Draft.MinX = *r_MinX;

			auto r_MinY = _Input.Read<double>();
			if (!r_MinY)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Box.MinY"));
			Draft.MinY = *r_MinY;

			auto r_MinZ = _Input.Read<double>();
			if (!r_MinZ)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Box.MinZ"));
			Draft.MinZ = *r_MinZ;

			auto r_IsValid = _Input.Read<uint8_t>();
			if (!r_IsValid)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read Box.IsValid"));
			Draft.IsValid = *r_IsValid;

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::FluidBox:
		{
			Struct::FluidBox Draft;
			auto r_Value = _Input.Read<float>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read FluidBox.Value"));
			Draft.Value = *r_Value;

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::InventoryItem:
		{
			Struct::InventoryItem Draft;

			auto r_ItemName = _Input.Read<Core::String>();
			if (!r_ItemName)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read InventoryItem.ItemName"));
			Draft.ItemName = *r_ItemName;

			auto r_HasProperty = _Input.Read<uint32_t>();
			if (!r_HasProperty)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read InventoryItem.HasProperty"));
			Draft.HasProperty = *r_HasProperty;

			auto r_ItemType = _Input.Read<Core::String>();
			if (!r_ItemType)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read InventoryItem.ItemType"));
			Draft.ItemType = *r_ItemType;

			if (Draft.HasProperty != 0)
			{
				auto r_Properties = _Input.Read<PropertyList>();
				if (!r_Properties)
					return std::unexpected(r_Properties.error());
				Draft.Properties = std::move(*r_Properties);
			}

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::RailroadTrackPosition:
		{
			Struct::RailroadTrackPosition Draft;
			auto r_Reference = _Input.Read<Core::ObjectReference>();
			if (!r_Reference)
				return std::unexpected(r_Reference.error()); Draft.Reference = *r_Reference;

			auto r_Offset = _Input.Read<float>();
			if (!r_Offset)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read RailroadTrackPosition.Offset"));
			Draft.Offset = *r_Offset;

			auto r_Forward = _Input.Read<float>();
			if (!r_Forward)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read RailroadTrackPosition.Forward"));
			Draft.Forward = *r_Forward;

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::DateTime:
		{
			Struct::DateTime Draft;
			auto r_Value = _Input.Read<int64_t>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read DateTime.Value"));
			Draft.Value = *r_Value;

			return Struct::Type{ Draft };
		}
		case Struct::ElementType::ClientIdentityInfo:
		{
			Struct::ClientIdentity Draft;

			auto r_UUID = _Input.Read<Core::String>();
			if (!r_UUID)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read ClientIdentity.UUID"));
			Draft.UUID = *r_UUID;

			uint64_t Consumed = _Input.GetBytesRead();
			uint64_t Remaining = (Consumed < _PayloadEnd) ? (_PayloadEnd - Consumed) : 0;
			Draft.Unused.resize(Remaining);
			if (Remaining > 0 and !_Input.ReadBytes(reinterpret_cast<char*>(Draft.Unused.data()), Remaining))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read ClientIdentity.Unused"));

			return Struct::Type{ std::move(Draft) };
		}
		case Struct::ElementType::Generic:
		default:
		{
			// Not one of the fixed types above (or the type name wasn't recognized at all):
			// a plain, self-terminating nested PropertyList.
			Struct::Generic Draft;
			auto r_Properties = _Input.Read<PropertyList>();
			if (!r_Properties)
				return std::unexpected(r_Properties.error());
			Draft.Value = std::move(*r_Properties);
			return Struct::Type{ std::move(Draft) };
		}
		}
	}

	static Result<StructProperty> ReadStructPayload(InputBlueprint& _Input, const Core::String& _Name, const Core::String& _TypeName, uint64_t _PayloadEnd)
	{
		StructProperty Draft;
		Draft.Name = _Name;

		auto r_Type = StrToEnum<Struct::ElementType>(_TypeName.Content);
		if (r_Type)
		{
			Draft.IsTypeGeneric = false;
			Draft.Type = *r_Type;
		}
		else
		{
			Draft.IsTypeGeneric = true;
			Draft.OriginalType = _TypeName;
			Draft.Type = Struct::ElementType::Generic;
		}

		auto r_Body = ReadStructBody(_Input, Draft.Type, _PayloadEnd);
		if (!r_Body)
			return std::unexpected(r_Body.error());
		Draft.Value = std::move(*r_Body);

		return Draft;
	}

#pragma endregion Struct payload

#pragma region Property / PropertyList

	template<>
	Result<Property> InputBlueprint::Read()
	{
		Property Draft;

		auto r_Name = Read<Core::String>();
		if (!r_Name)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property name"));
		Draft.Name = *r_Name;

		if (Draft.Name.Content == "None") // Sentinel that terminates a PropertyList; caller checks for it
			return Draft;

		auto r_TypeName = Read<Core::String>();
		if (!r_TypeName)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property type name"));

		auto r_Type = StrToEnum<PropertyType>(r_TypeName->Content);
		if (!r_Type)
			return std::unexpected(r_Type.error());
		Draft.Type = *r_Type;

		auto r_Length = Read<uint32_t>();
		if (!r_Length)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property length"));

		auto r_Index = Read<uint32_t>();
		if (!r_Index)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property index"));
		Draft.Index = *r_Index;

		// A handful of property types carry an extra tag field here (before the pad byte):
		// BoolProperty's value is inline in the tag (no separate payload at all), Byte/Enum
		// carry an enum type name, Array/Set carry their element type name, Struct carries its
		// struct type name, Map carries both a key and a value type name.

		uint8_t BoolValue{};
		Core::String ExtraTypeName{};
		Core::String ExtraValueTypeName{};

		switch (Draft.Type)
		{
		case PropertyType::Bool:
		{
			auto r_Value = Read<uint8_t>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read bool property value"));
			BoolValue = *r_Value;
			break;
		}
		case PropertyType::Byte:
		case PropertyType::Enum:
		case PropertyType::Array:
		case PropertyType::Set:
		case PropertyType::Struct:
		{
			auto r_Extra = Read<Core::String>();
			if (!r_Extra)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property sub-type name"));
			ExtraTypeName = *r_Extra;
			break;
		}
		case PropertyType::Map:
		{
			auto r_Key = Read<Core::String>();
			if (!r_Key)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map key type name"));
			ExtraTypeName = *r_Key;

			auto r_Value = Read<Core::String>();
			if (!r_Value)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read map value type name"));
			ExtraValueTypeName = *r_Value;
			break;
		}
		default:
			break;
		}

		if (Draft.Type == PropertyType::Struct)
			SkipBytes(16); // Struct GUID: two Longs, always zero, per the wiki's StructProperty table

		auto r_PadByte = Read<uint8_t>();
		if (!r_PadByte)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property pad byte"));

		uint64_t PayloadStart = GetBytesRead();
		uint64_t PayloadEnd = PayloadStart + *r_Length;

		switch (Draft.Type)
		{
		case PropertyType::Bool:
			Draft.Value = BoolProperty{ BoolValue };
			break;

		case PropertyType::Byte:
		{
			auto r_Byte = ReadBytePayload(*this, ExtraTypeName);
			if (!r_Byte) return std::unexpected(r_Byte.error());
			Draft.Value = std::move(*r_Byte);
			break;
		}
		case PropertyType::Enum:
		{
			auto r_Enum = ReadEnumPayload(*this);
			if (!r_Enum) return std::unexpected(r_Enum.error());
			Draft.Value = std::move(*r_Enum);
			break;
		}
		case PropertyType::Float:
		{
			auto r_Float = Read<FloatProperty>();
			if (!r_Float) return std::unexpected(r_Float.error());
			Draft.Value = std::move(*r_Float);
			break;
		}
		case PropertyType::Double:
		{
			auto r_Double = Read<DoubleProperty>();
			if (!r_Double) return std::unexpected(r_Double.error());
			Draft.Value = std::move(*r_Double);
			break;
		}
		case PropertyType::Int:
		{
			auto r_Int = Read<IntProperty>();
			if (!r_Int) return std::unexpected(r_Int.error());
			Draft.Value = std::move(*r_Int);
			break;
		}
		case PropertyType::Int8:
		{
			auto r_Int8 = Read<Int8Property>();
			if (!r_Int8) return std::unexpected(r_Int8.error());
			Draft.Value = std::move(*r_Int8);
			break;
		}
		case PropertyType::UInt32:
		{
			auto r_UInt32 = Read<Uint32Property>();
			if (!r_UInt32) return std::unexpected(r_UInt32.error());
			Draft.Value = std::move(*r_UInt32);
			break;
		}
		case PropertyType::Int64:
		{
			auto r_Int64 = Read<Int64Property>();
			if (!r_Int64) return std::unexpected(r_Int64.error());
			Draft.Value = std::move(*r_Int64);
			break;
		}
		case PropertyType::Name:
		{
			auto r_NameValue = Read<NameProperty>();
			if (!r_NameValue) return std::unexpected(r_NameValue.error());
			Draft.Value = std::move(*r_NameValue);
			break;
		}
		case PropertyType::Object:
		{
			auto r_Object = Read<ObjectProperty>();
			if (!r_Object) return std::unexpected(r_Object.error());
			Draft.Value = std::move(*r_Object);
			break;
		}
		case PropertyType::SoftObject:
		{
			auto r_SoftObject = Read<SoftObjectProperty>();
			if (!r_SoftObject) return std::unexpected(r_SoftObject.error());
			Draft.Value = std::move(*r_SoftObject);
			break;
		}
		case PropertyType::Str:
		{
			auto r_Str = Read<StrProperty>();
			if (!r_Str) return std::unexpected(r_Str.error());
			Draft.Value = std::move(*r_Str);
			break;
		}
		case PropertyType::Text:
		{
			auto r_Text = Read<TextProperty>();
			if (!r_Text) return std::unexpected(r_Text.error());
			Draft.Value = std::move(*r_Text);
			break;
		}
		case PropertyType::Array:
		{
			auto r_Array = ReadArrayPayload(*this, Draft.Name, ExtraTypeName);
			if (!r_Array) return std::unexpected(r_Array.error());
			Draft.Value = std::move(*r_Array);
			break;
		}
		case PropertyType::Map:
		{
			auto r_Map = ReadMapPayload(*this, Draft.Name, ExtraTypeName, ExtraValueTypeName);
			if (!r_Map) return std::unexpected(r_Map.error());
			Draft.Value = std::move(*r_Map);
			break;
		}
		case PropertyType::Set:
		{
			auto r_Set = ReadSetPayload(*this, Draft.Name, ExtraTypeName);
			if (!r_Set) return std::unexpected(r_Set.error());
			Draft.Value = std::move(*r_Set);
			break;
		}
		case PropertyType::Struct:
		{
			auto r_Struct = ReadStructPayload(*this, Draft.Name, ExtraTypeName, PayloadEnd);
			if (!r_Struct) return std::unexpected(r_Struct.error());
			Draft.Value = std::move(*r_Struct);
			break;
		}
		}

		// Defensive resync: Length is authoritative. If a payload reader consumed a different
		// number of bytes than advertised, snap back into alignment for the next property
		// instead of letting the mistake cascade into everything that follows.

		uint64_t Consumed = GetBytesRead() - PayloadStart;
		if (Consumed < *r_Length)
			SkipBytes(*r_Length - Consumed);

		return Draft;
	}

	template<>
	Result<PropertyList> InputBlueprint::Read()
	{
		PropertyList Draft;

		while (true)
		{
			auto r_Property = Read<Property>();
			if (!r_Property)
				return std::unexpected(r_Property.error());

			if (r_Property->Name.Content == "None")
				break;

			AppendProperty(Draft, std::move(*r_Property));
		}

		return Draft;
	}

#pragma endregion Property / PropertyList
}
