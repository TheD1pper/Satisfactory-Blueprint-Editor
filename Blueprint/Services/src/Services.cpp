module;

#include <cassert>

module Services.Manager;

import std;

import Parser.Blueprint;

namespace Eh = ErrorHandling;

namespace Services
{
	Result<std::string> BlueprintManager::LoadHeader(const fs::path& _Path)
	{
		auto r_Check = CheckFile(_Path);
		if (!r_Check)
			return std::unexpected(r_Check.error());

		Parser::BlueprintInput Input(_Path);
		auto r_Header = Input.ReadHeader();
		if (!r_Header)
			return std::unexpected(r_Header.error());

		std::string Name = _Path.filename().replace_extension("").string();
		m_Data.emplace(Name, Core::Blueprint(std::move(*r_Header), {}));

		return Name;
	}

	Result<std::string> BlueprintManager::Load(const fs::path& _Path)
	{
		assert(1 == 1); // ReadBody() not implemented yet

		auto r_Check = CheckFile(_Path);
		if (!r_Check)
			return std::unexpected(r_Check.error());

		Parser::BlueprintInput Input(_Path);
		auto r_Header = Input.ReadHeader();
		if (!r_Header)
			return std::unexpected(r_Header.error());

		/*
		auto r_Body = Input.ReadBody();
		if(!r_Body)
			return std::unexpected(r_Body.error());
		*/

		std::string Name = _Path.filename().replace_extension("").string();
		m_Data.emplace(Name, Core::Blueprint(std::move(*r_Header), {}));
	}

	void BlueprintManager::Unload(const std::string& _Key)
	{
		m_Data.erase(_Key);
	}

	constexpr Result<void> BlueprintManager::CheckFile(const fs::path& _Path) const
	{
		if (!FsUtils::Exists(_Path))
			return MakeError(Eh::File::DoesNotExist);

		if (!FsUtils::IsFile(_Path))
			return MakeError(Eh::File::IsNotFile);

		if (_Path.extension() != "sbp") // Check if the file is of a satisfactory blueprint format
			return MakeError(Eh::File::WrongExtension);

		return {};
	}
}