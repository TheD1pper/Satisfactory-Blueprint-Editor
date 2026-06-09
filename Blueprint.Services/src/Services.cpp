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

	std::string GetBlueprintsString()
	{
		auto Blueprints = Parser::GetFolderContents(Parser::BlueprintFolder);
		std::string Message;
		for (int i{ 0 }; i < Blueprints.size(); i++)
		{
			if (Blueprints[i].extension().string() == ".sbp")
				Message = Message + std::format("{}) {}\n", i, Blueprints[i].filename().string());
		}
		return Message;
	}

	
}