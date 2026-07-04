#pragma once

#include <string>
#include <cstdint>

namespace Core
{
	using Byte = char;
	using Int = int;
	using Uint32 = uint32_t;
	using Uint64 = uint64_t;
	using Float = float;
	using Double = double;

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
		std::string Utf16ToUtf8(std::u16string_view _String);
		std::u16string Utf16ToUtf8(std::string_view _String);
	};

	using String = BinaryString;
}
