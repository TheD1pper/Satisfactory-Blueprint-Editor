module;

module Parser.Compression;

import <zlib.h>;

import Helpers.Errors;
import Core.Data;

namespace Parser
{
	Result<Core::ByteVector> Zlib::Compress(const Core::ByteVector& _Input)
	{
		uLongf CompressedSize = compressBound(static_cast<uLong>(_Input.size()));
		Core::ByteVector Output(CompressedSize);

		if (compress(Output.data(), &CompressedSize, _Input.data(), static_cast<uLong>(_Input.size())) != Z_OK)
			return std::unexpected(Eh::Error(Eh::Compression::Fail, "Compression failed"));

		Output.resize(CompressedSize);
		return Output;
	}

	Result<Core::ByteVector> Zlib::Decompress(const Core::ByteVector& _Input, const size_t& UncompressedSize)
	{
		Core::ByteVector Output(UncompressedSize);
		uLongf Size = static_cast<uLongf>(UncompressedSize);

		if (uncompress(Output.data(), &Size, _Input.data(), static_cast<uLong>(_Input.size())) != Z_OK)
			return std::unexpected(Eh::Error(Eh::Compression::Fail, "Decompression failed"));

		Output.resize(Size);
		return Output;
	}
}