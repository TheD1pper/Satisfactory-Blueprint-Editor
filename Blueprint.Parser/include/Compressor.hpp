#pragma once

#include <zlib.h>
#include <vector>
#include <string>

#include "BasicDataTypes.hpp"
#include "Parser.hpp"

import Helpers.Errors;

namespace Parser
{
	using ByteVector = std::vector<uint8_t>;

	class Zlib
	{
	public:
		Result<ByteVector> Compress(const ByteVector& _Input);
		Result<ByteVector> Decompress(const ByteVector& _Input, const size_t& UncompressedSize);
	};
	extern Zlib Z;
}