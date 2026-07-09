#include <iostream>
#include <print>

#include "Parser.hpp"
#include "BasicDataTypes.hpp"

namespace Parser
{
	InputBlueprint::InputBlueprint(fs::path& _Path)
	{
		Input = std::move(std::ifstream(std::move(_Path)));
	}

	template <>
	Result<Core::Int> InputBlueprint::Read()
	{
		Core::Int Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Int read"));
		return Value;
	}
	
	template <>
	Result<Core::Uint32> InputBlueprint::Read()
	{
		Core::Uint32 Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint32 read"));
		return Value;
	}

	template <>
	Result<Core::Uint64> InputBlueprint::Read()
	{
		Core::Uint64 Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 8))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint64 read"));
		return Value;
	}

	template <>
	Result<Core::String> InputBlueprint::Read()
	{
		auto r_Length = Read<Core::Int>();

		if (!r_Length.has_value())
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad string length read"));

		Core::Int SignedLength = r_Length.value();

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
}