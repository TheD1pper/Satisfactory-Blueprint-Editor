module;

export module Services.Manager;

import <memory>;
import <filesystem>;
import <unordered_map>;

import Helpers.Errors;
import Core.Data;
import Parser.BinaryIO;

namespace fs = std::filesystem;

namespace Services
{
	// _Path is only temporary, switch to blueprint names from .sbpcfg files in near future
	export class BlueprintManager
	{
	private:
		std::unordered_map<std::string, Core::Blueprint> m_Table;

	public:
		Core::Blueprint& operator[](const fs::path& _Key);
		Result<std::string> Load(const fs::path& _Path);
		void Drop(const fs::path& _Key);
		Result<void> Write(const fs::path& _Key, const fs::path& _Path);
		bool Contains(const fs::path& _Path);
	};
}