#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace Core
{
	enum class Encoding
	{
		Utf8, Utf16
	};

	struct BinaryString
	{
		Encoding Utf{};
		std::string Content;

		std::u16string Utf8ToUtf16(std::string_view _String);
		std::string Utf16ToUtf8(std::u16string_view _String);
	};

	using String = BinaryString;
	using ByteVector = std::vector<uint8_t>;

	struct Size3D
	{
		uint32_t X{};
		uint32_t Y{};
		uint32_t Z{};
	};

	struct Rotation3D
	{
		float X{};
		float Y{};
		float Z{};
	};

	struct Rotation4D
	{
		float X{};
		float Y{};
		float Z{};
		float W{};
	};

	struct Position3D
	{
		float X{};
		float Y{};
		float Z{};
	};

	struct Scale3D
	{
		float X{};
		float Y{};
		float Z{};
	};

	struct FPackageFileVersion
	{
		int FileVersionUE4{};
		int FileVersionUE5{};
	};

	struct FEngineVersion
	{
		uint16_t Major{};
		uint16_t Minor{};
		uint16_t Patch{};
		uint32_t ChangeList{};
		String Branch{};
	};

	struct ObjectReference
	{
		String Level;
		String Path;
	};

	struct ContentEntry
	{
		String ClassName;
	};
}
