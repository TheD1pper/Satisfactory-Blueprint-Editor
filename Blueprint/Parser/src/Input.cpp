module;

#include <cassert>

module Parser.Blueprint:Input;

import std;

import Helpers.Errors;
import Core.Types;

namespace Eh = ErrorHandling;

namespace Parser
{
	uint64 BlueprintInput::GetBytesRead()
	{
		return m_Input.gcount();
	}

	std::istream& BlueprintInput::ReadBytes(char* _Ptr, uint64 _Count)
	{
		m_Input.read(_Ptr, _Count);
		return m_Input;
	}

	void BlueprintInput::SkipBytes(uint64 _Count)
	{
		m_Input.ignore(_Count);
	}

	template<typename T>
	Result<T> BlueprintInput::Read()
	{
		static_assert(std::is_arithmetic_v<T>, "T is not trivial; provide an explicit BlueprintInput::Read<T>() specialization");

		if (T Value{}; !ReadBytes(reinterpret_cast<char*>(&Value), sizeof(T)))
			return Eh::MakeError(Eh::Binary::BadRead);
		else
			return Value;
	}

	template<>
	Result<Core::String> BlueprintInput::Read()
	{
		Core::String Draft;

		auto r_Length = Read<int32>();
		if (!r_Length)
			return Eh::MakeError(Eh::Binary::BadRead, "Could not read string length");

		if (*r_Length == 0)
			return Eh::MakeError(Eh::Binary::EmptyString, "Empty string");

		if (*r_Length > 0)
		{
			std::string Data;
			Data.resize(*r_Length);

			if (!ReadBytes(Data.data(), *r_Length))
				return Eh::MakeError(Eh::Binary::BadRead, "Could not read UTF-8 string data");

			SkipBytes(1); // Skip null-terminator

			Draft.Data = std::move(Data);
			Draft.Encoding = Core::StrEncoding::Utf8;
		}

		if (*r_Length < 0)
		{
			assert(true);
			std::wstring Data;
			Data.resize(*r_Length);

			if (!ReadBytes(reinterpret_cast<char*>(Data.data()), *r_Length))
				return Eh::MakeError(Eh::Binary::BadRead, "Could not read UTF-16 string data");

			SkipBytes(2); // Skip null-terminator

			Draft.Data = std::move(Data);
			Draft.Encoding = Core::StrEncoding::Utf16;
		}

		return Draft;
	}
}