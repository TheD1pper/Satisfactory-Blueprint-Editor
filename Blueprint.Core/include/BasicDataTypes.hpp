#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace Core
{
	using Byte = uint8_t;
	using Int = int;
	using Uint16 = uint16_t;
	using Uint32 = uint32_t;
	using Uint64 = uint64_t;
	using Float = float;
	using Double = double;

	enum class Encoding
	{
		Utf8, Utf16
	};

	class BinaryString
	{
	public:
		Encoding Utf{};
		std::string Content;

	public:
		std::u16string Utf8ToUtf16(std::string_view _String);
		std::string Utf16ToUtf8(std::u16string_view _String);

	public:
		std::string String() const;
	};

	using String = BinaryString;

	struct Size3D
	{
	public:
		Uint32 X{};
		Uint32 Y{};
		Uint32 Z{};
	};

	struct Rotation3D
	{
	public:
		Float X{};
		Float Y{};
		Float Z{};
	};

	struct Position3D
	{
	public:
		Float X{};
		Float Y{};
		Float Z{};
	};

	struct Scale3D
	{
	public:
		Float X{};
		Float Y{};
		Float Z{};
	};

	struct FPackageFileVersion
	{
		Int FileVersionUE4{};
		Int FileVersionUE5{};
	};

	struct FEngineVersion
	{
		Uint16 Major{};
		Uint16 Minor{};
		Uint16 Patch{};
		Uint32 ChangeList{};
		String Branch{};
	};

	class UnknownData
	{
	private:
		std::vector<Core::Byte> Bytes;

	public:


	};

	
}
