#include <iostream>
#include <zlib.h>
#include <print>

#include "Parser.hpp"
#include "BasicDataTypes.hpp"

namespace Parser
{
	InputBlueprint::InputBlueprint(fs::path _Path)
	{
		Input = std::move(std::ifstream(std::move(_Path), std::ios::binary));
	}

	template <>
	Result<uint8_t> InputBlueprint::Read()
	{
		uint8_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 1))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Byte read"));
		return Value;
	}

	template <>
	Result<uint16_t> InputBlueprint::Read()
	{
		uint16_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 2))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint16 read"));
		return Value;
	}

	template <>
	Result<int> InputBlueprint::Read()
	{
		int Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Int read"));
		return Value;
	}
	
	template <>
	Result<uint32_t> InputBlueprint::Read()
	{
		uint32_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint32 read"));
		return Value;
	}

	template <>
	Result<uint64_t> InputBlueprint::Read()
	{
		uint64_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 8))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint64 read"));
		return Value;
	}

	template <>
	Result<Core::String> InputBlueprint::Read()
	{
		auto r_Length = Read<int>();

		if (!r_Length)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad string length read"));

		int SignedLength = r_Length.value();

		if (SignedLength == 0)
			return std::unexpected(Eh::Error(Eh::Binary::EmptyString, "Empty string"));

		Core::String Value{};

		if (SignedLength > 0) // UTF-8
		{
			Value.Utf = Core::Encoding::Utf8;
			uint32_t Length = static_cast<uint32_t>(SignedLength);

			Value.Content = std::string(Length - 1, '\0');

			if (!ReadBytes(Value.Content.data(), Length - 1))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad u8string read"));

			SkipBytes(1); // Ignore null-termination
			return Value;
		}
		else if (SignedLength < 0) // UTF-16
		{
			Value.Utf = Core::Encoding::Utf16;
			uint32_t ByteLength = static_cast<uint32_t>(-SignedLength);

			Value.Content = std::string(ByteLength - 2, '\0');

			if (!ReadBytes(Value.Content.data(), ByteLength - 2))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad u16string read"));

			SkipBytes(2); // Ignore null-termination
			return Value;
		}

		return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad string length read"));
	}

	uint64_t InputBlueprint::GetBytesRead() const
	{
		return BytesRead;
	}

	void InputBlueprint::SkipBytes(std::streamsize _Bytes)
	{
		BytesRead += _Bytes;
		Input.ignore(_Bytes);

	}

	std::istream& InputBlueprint::ReadBytes(char* _String, uint64_t _Count)
	{
		Input.read(_String, _Count);
		BytesRead += Input.gcount();
		return Input;
	}

	bool BinaryOutput::Write(std::string& _Data)
	{
		BytesWritten += _Data.size();
		if(!Output.write(_Data.data(), _Data.size()))
			return false;
		return true;
	}

	inline std::streamsize BinaryOutput::GetBytesWritten() const
	{
		return BytesWritten;
	}

	BinaryOutput::BinaryOutput(fs::path _Path)
	{
		Output = std::ofstream(_Path, std::ios::binary);
	}
}