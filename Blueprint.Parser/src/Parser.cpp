module;

#include <iostream>
#include <zlib.h>
#include <print>

module Parser.BinaryIO;

import Helpers.Benchmark;
import Helpers.Errors;
import Helpers.FsUtils;
import Core.Data;
import Core.Property;

using namespace Core::Property;

namespace Parser
{
#pragma region Read implementations
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
#pragma endregion Read() implementations of basic data types

#pragma region Property implementations

	template<>
	Result<Property> InputBlueprint::Read()
	{
		Property Draft;

		auto r_Name = Read<Core::String>();
		if (!r_Name)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property name"));
		Draft.Name = *r_Name;

		auto r_StringType = Read <Core::String>();
		if (r_StringType)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read property type"));
		

		return Draft;
	}

#pragma endregion Read() implementations of properties

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

	fs::path InputBlueprint::GetPath()
	{
		return Path;
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

	InputBlueprint::InputBlueprint(const fs::path& _Path)
	{
		Path = _Path;
		Input = std::move(std::ifstream(Path, std::ios::binary));
	}

	BinaryOutput::BinaryOutput(fs::path _Path)
	{
		Output = std::ofstream(_Path, std::ios::binary);
	}
}