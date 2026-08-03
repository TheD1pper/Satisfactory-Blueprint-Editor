module;

#include <cassert>
#include <iostream>

#include "BenchmarkMacros.hpp"

module Services.Manager;

import Helpers.Benchmark;
import Helpers.FsUtils;
import Helpers.Errors;
import Core.Data;
import Parser.BinaryIO;

namespace Services
{
	// Can return the blueprint name in the future
	// Replace paths as keys for blueprint names 
	Result<std::string> BlueprintManager::Load(const fs::path& _Path)
	{
		Benchmark::Instrumentor::Get().BeginSession("Blueprint loading");
		BENCH_FUNC();

		Parser::InputBlueprint Input(_Path);
		Core::Blueprint Blueprint{};

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

			Blueprint.Header = std::move(*r_Header);
			Blueprint.Body = std::move(*r_Body);
			m_Table.emplace(_Path.filename().string(), std::move(Blueprint));
		}

		return _Path.string();
	}

	void BlueprintManager::Drop(const fs::path& _Key)
	{
		m_Table.erase(_Key.filename().string());
	}

	bool BlueprintManager::Contains(const fs::path& _Key)
	{
		return m_Table.contains(_Key.filename().string());
	}

	Core::Blueprint& BlueprintManager::operator[](const fs::path& _Key)
	{
		return m_Table[_Key.filename().string()];
	}

	Result<void> BlueprintManager::Write(const fs::path& _Key, const fs::path& _Path)
	{
		assert(1 == 1);
		return {};
	}
}