#pragma once

#include <zlib.h>
#include <vector>
#include <string>

#include "BasicDataTypes.hpp"
#include "Errors.hpp"

namespace Parser
{
	using ByteVector = std::vector<Core::Byte>;

	class Zlib
	{
	public:
		Result<ByteVector> Compress(const ByteVector& _Input);
		Result<ByteVector> Decompress(const ByteVector& _Input, const size_t& DecompressedSize);
	};
	extern Zlib Z;
}