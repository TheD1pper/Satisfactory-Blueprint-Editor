#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <array>
#include <vector>
#include <cstdint>

#include "Parser.hpp"
#include "Errors.hpp"

namespace Parser
{
	InputFile::InputFile(fs::path _Path)
	{
		Input = std::ifstream(Path, std::ios::binary);
		Path = _Path;
	}
	
	Result<uint32_t> InputFile::Read()
	{
		uint32_t value = 0;

		if (!Input.read(reinterpret_cast<char*>(&value), sizeof(value)))
		{
			return 0;
		}

		return value;
	}

	fs::path GetLocalAppdata()
	{
		char* buf = nullptr;
		size_t sz = 0;
		if (_dupenv_s(&buf, &sz, "localappdata") == 0 && buf != nullptr)
			return (fs::path)buf;
		return "";
		
	}

	fs::path GetBlueprintFolder()
	{
		return LocalAppData / "FactoryGame\\Saved\\SaveGames\\blueprints";
	}

	std::vector<fs::path> GetFolderContents(const fs::path& path)
	{
		std::vector<fs::path> contents;
		for (const auto& entry : fs::directory_iterator(path))
			contents.push_back(entry.path());
		return contents;
	}



	fs::path LocalAppData = GetLocalAppdata();
	fs::path BlueprintFolder = GetBlueprintFolder();
}