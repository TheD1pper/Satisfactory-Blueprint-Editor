module;

export module Services.Manager;

import <memory>;
import <filesystem>;
import <unordered_map>;

import Helpers.Errors;
import Core.Data;
import Core.Complex;
import Parser.BinaryIO;

namespace fs = std::filesystem;

namespace Services
{
	export class BlueprintManager
	{
	private:
		std::unordered_map<std::string, Core::Blueprint> m_Table;

	public:
		Core::Blueprint& operator[](const std::string& _Key);
		Result<std::string> Load(const fs::path& _Path);
		void Drop(const std::string& _Key);
		Result<void> Write(const std::string& _Key, const fs::path& _Path);
		bool Contains(const std::string& _Path);
	};
}