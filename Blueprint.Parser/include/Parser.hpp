#pragma once

#include <fstream>
#include <filesystem>

#include "Core.hpp"
#include "Errors.hpp"

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


	private:
		template <typename T> Result<T> Read();
		void SkipBytes(std::streamsize _Bytes);
		std::istream& ReadBytes(char* _String, uint64_t _Count);

	private:

	public:
		InputBlueprint() = default;
		InputBlueprint(fs::path _Path);

	};
}