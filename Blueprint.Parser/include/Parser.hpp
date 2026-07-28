#pragma once

#include <fstream>
#include <filesystem>

#include "Core.hpp"
#include "Compressor.hpp"

import Helpers.Errors;

namespace fs = std::filesystem;

namespace Parser
{
	int constexpr MaxChunkSize = 0x00020000;

	class InputBlueprint
	{
	private:
		uint64_t BytesRead = 0;
		std::ifstream Input;

	public:
		Result<Core::BlueprintHeader> ReadHeader();
		Result<Core::BlueprintBody> ReadBody();

		uint64_t GetBytesRead() const;
		template <typename T> Result<T> Read();
		void SkipBytes(std::streamsize _Bytes);
		std::istream& ReadBytes(char* _String, uint64_t _Count);

	private:

	public:
		InputBlueprint() = default;
		InputBlueprint(fs::path _Path);

	};

	class BinaryOutput
	{
	private:
		std::ofstream Output;
		std::streamsize BytesWritten = 0;

	public:
		bool Write(std::string& _Data);
		inline std::streamsize GetBytesWritten() const;

	public:
		BinaryOutput() = default;
		BinaryOutput(fs::path _Path);
	};
}