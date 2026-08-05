module;

#include <cassert>

#include "BenchmarkMacros.hpp"

module Services.Manager;

import Helpers.Benchmark;
import Helpers.FsUtils;
import Helpers.Errors;
import Core.Data;
import Parser.BinaryIO;

namespace Services
{
	Result<std::string> BlueprintManager::Load(const fs::path& _Path)
	{
		Benchmark::Instrumentor::Get().BeginSession("Blueprint loading");
		BENCH_FUNC();

		Parser::InputBlueprint Input(_Path);
		Core::Blueprint Blueprint{};

		const std::string BlueprintName = _Path.filename().replace_extension("").string();

		{
			BENCH_SCOPE("Blueprint read");
			Result<Core::BlueprintHeader> r_Header;
			{
				BENCH_SCOPE("Header read");
				r_Header = Input.ReadHeader();
				if (!r_Header)
					return std::unexpected(r_Header.error());
			}

			Result<Core::BlueprintBody> r_Body;
			{
				BENCH_SCOPE("Body read");
				r_Body = Input.ReadBody();
				if (!r_Body)
					return std::unexpected(r_Body.error());
			}

			Blueprint.Name = BlueprintName;
			Blueprint.Header = std::move(*r_Header);
			Blueprint.Body = std::move(*r_Body);
			m_Table.emplace(Blueprint.Name, std::move(Blueprint));
		}

		return BlueprintName;
	}

	void BlueprintManager::Drop(const std::string& _Key)
	{
		m_Table.erase(_Key);
	}

	bool BlueprintManager::Contains(const std::string& _Key)
	{
		return m_Table.contains(_Key);
	}

	Core::Blueprint& BlueprintManager::operator[](const std::string& _Key)
	{
		return m_Table[_Key];
	}

	Result<void> BlueprintManager::Write(const std::string& _Key, const fs::path& _Path)
	{
		assert(1 == 1);
		return {};
	}
}