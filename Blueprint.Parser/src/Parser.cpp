#include <iostream>
#include <print>

#include "Parser.hpp"
#include "BasicDataTypes.hpp"

namespace Parser
{

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