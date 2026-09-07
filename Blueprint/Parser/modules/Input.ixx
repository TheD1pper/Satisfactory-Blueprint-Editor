module;

export module Parser.Blueprint:Input;

import std;

import Helpers.Errors;
import Helpers.FsUtils;
import Core.Types;

import :Constants;

namespace Parser
{
	export class BlueprintInput
	{
	public:
		BlueprintInput(const fs::path& _Path);

		Result<Core::BlueprintHeader> ReadHeader();

	private:
		std::ifstream m_Input;

	private:
		template<typename T> Result<T> Read();
		uint64 GetPtrPos();
		std::istream& ReadBytes(char* _Ptr, uint64 _Count);
		inline void SkipBytes(uint64 _Count);
	};
}