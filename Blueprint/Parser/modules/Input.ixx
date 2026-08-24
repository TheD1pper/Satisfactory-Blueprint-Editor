module;

export module Parser.Blueprint:Input;

import std;

import Helpers.Errors;
import Core.Types;

namespace Parser
{
	export class BlueprintInput
	{
	public:
		template<typename T>
		Result<T> Read();

	private:
		std::ifstream m_Input;

	private:
		uint64 GetBytesRead();
		std::istream& ReadBytes(char* _Ptr, uint64 _Count);
		void SkipBytes(uint64 _Count);
	};
}