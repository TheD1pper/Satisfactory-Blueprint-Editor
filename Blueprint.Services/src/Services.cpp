#include <iostream>
#include <unordered_map>
#include <format>
#include <print>

#include "Services.hpp"
#include "Parser.hpp"

//
// Here will lie as follows:
// Main Logic
//

namespace Services
{
	std::string GetSavesString()
	{
		auto Blueprints = Parser::GetFolderContents(Parser::BlueprintFolder);
		std::string Message;
		for (int i{ 0 }; i < Blueprints.size(); i++)
		{
			Message = Message + std::format("{}) {}\n", i, Blueprints[i].filename().string());
		}
		return Message;
	}

	std::string GetBlueprintsString(fs::path _Path)
	{
		auto Blueprints = Parser::GetFolderContents(Parser::BlueprintFolder / _Path);
		std::string Message;
		int index = 0;
		for(const auto& Blueprint : Blueprints)
		{
			if (Blueprint.extension().string() == ".sbp")
			{
				index++;
				Message = Message + std::format("{}) {}\n", index, Blueprint.filename().replace_extension("").string());
			}
		}
		return Message;
	}

	
}