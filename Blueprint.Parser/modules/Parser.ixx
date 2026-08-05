module;

export module Parser.BinaryIO;

import <fstream>;
import <filesystem>;

import Helpers.Errors;
import Core.Data;

namespace fs = std::filesystem;

export namespace Parser
{
	int constexpr MaxChunkSize = 0x00020000;

	export class InputBlueprint
	{
	private:
		uint64_t BytesRead = 0;
		fs::path Path{};
		std::ifstream Input;

	public:
		Result<Core::BlueprintHeader> ReadHeader();
		Result<Core::BlueprintBody> ReadBody();

		uint64_t GetBytesRead() const;
		template <typename T> Result<T> Read();
		void SkipBytes(std::streamsize _Bytes);
		std::istream& ReadBytes(char* _String, uint64_t _Count);
		fs::path GetPath();

	private:

	public:
		InputBlueprint() = default;
		InputBlueprint(const fs::path& _Path);
	};

	export class BinaryOutput
	{
	private:
		std::ofstream Output;
		std::streamsize BytesWritten = 0;

	public:
		bool Write(std::string& _Data);
		bool Write(Core::ByteVector _Data);
		inline std::streamsize GetBytesWritten() const;

	public:
		BinaryOutput() = default;
		BinaryOutput(fs::path _Path);
	};
}