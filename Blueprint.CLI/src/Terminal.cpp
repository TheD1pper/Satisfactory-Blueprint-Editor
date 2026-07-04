#include <Windows.h>
#include <string>
#include <print>
#include <ctime>
#include <array>

#include "Services.hpp"
#include "Terminal.hpp"

namespace Cli
{
	void ClearTerminal()
	{
		system("cls");
	}

	void LoadBlueprint()
	{
		Services::ReadHeader();
	}
}