module;

#include "zlib.h"

export module Parser.Compression;

import <vector>;

import Core.Data;
import Helpers.Errors;

namespace Parser
{
	export class Zlib
	{
	public:
		Result<Core::ByteVector> Compress(const Core::ByteVector& _Input);
		Result<Core::ByteVector> Decompress(const Core::ByteVector& _Input, const size_t& UncompressedSize);
	};
}