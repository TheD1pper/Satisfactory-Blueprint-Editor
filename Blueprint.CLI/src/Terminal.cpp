#include <Windows.h>
#include <string>
#include <print>
#include <ctime>
#include <array>

#include "Terminal.hpp"

namespace Cli
{
	void ClearTerminal()
	{
		system("cls");
	}

	
	[[nodiscard]] std::string WelcomeMessage()
	{
		srand(time(NULL));
		std::array<std::string, 3> Messages{{"Satisfactory Blueprint Editor is up and running.\nWhat are your plans sir?\n",
			"Welcome to Satisfactory Blueprint Editor!\nWhat do you want to do?\n",
			"Satisfactory Blueprint Editor has launched succesfully!\nI'm waiting for your command.\n"}};
		int RandomIndex = rand() % Messages.size();
		return Messages[RandomIndex];

	}
}