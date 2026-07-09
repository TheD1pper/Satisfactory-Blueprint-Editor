

#include "Compressor.hpp"

namespace Parser
{
	Zlib Z;
	Result<ByteVector> Zlib::Compress(const ByteVector& _Input)
	{
		uLongf CompressedSize = compressBound(static_cast<uLong>(_Input.size()));
		ByteVector Output(CompressedSize);

		if (compress(Output.data(), &CompressedSize, _Input.data(), static_cast<uLong>(_Input.size())) != Z_OK)
			return std::unexpected(Eh::Error(Eh::Compression::Fail, "Compression failed"));

		Output.resize(CompressedSize);
		return Output;
	}

	Result<ByteVector> Zlib::Decompress(const ByteVector& _Input, const size_t& DecompressedSize)
	{
		ByteVector Output(DecompressedSize);
		uLongf Size = static_cast<uLongf>(DecompressedSize);

		if (uncompress(Output.data(), &Size, _Input.data(), static_cast<uLong>(_Input.size())) != Z_OK)
			return std::unexpected(Eh::Error(Eh::Compression::Fail, "Decompression failed"));

		Output.resize(Size);
		return Output;
	}
}