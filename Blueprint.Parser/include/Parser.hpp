#pragma once

#include <fstream>
#include <filesystem>

#include "Core.hpp"
#include "Errors.hpp"

namespace fs = std::filesystem;

namespace Parser
{
	class InputBlueprint
	{
	private:
		uint64_t BytesRead = 0;
		std::ifstream Input;

	public:
		uint64_t GetBytesRead() const;

	private:
		template <typename T> Result<T> Read();
		void SkipBytes(std::streamsize _Bytes);
		std::istream& ReadBytes(char* _String, uint64_t _Count);

	};
}