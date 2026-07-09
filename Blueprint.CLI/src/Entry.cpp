#include <print>
#include <iostream>

#include "Terminal.hpp"
#include "Services.hpp"
#include "Parser.hpp"

int main()
{
	Services::LoadBlueprint(FsUtils::BlueprintsPath.string() + "\\Exp 1.2\\Loop.sbp");
	Cli::PrintHeader(Services::LoadedBlueprint->Header);
	
} 