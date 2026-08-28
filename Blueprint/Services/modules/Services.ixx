module;

export module Services.Manager;

import std;

import Helpers.Errors;
import Helpers.FsUtils;
import Core.Types;

namespace Services
{
	export class BlueprintManager
	{
	public:
		Result<std::string> LoadHeader(const fs::path& _Path);
		Result<std::string> Load(const fs::path& _Path);
		void Unload(const std::string& _Key);

		Core::Blueprint& operator[](const std::string& _Key);

	private:
		std::unordered_map<std::string, Core::Blueprint> m_Data;

	private:
		constexpr Result<void> CheckFile(const fs::path& _Path) const;
	};
}