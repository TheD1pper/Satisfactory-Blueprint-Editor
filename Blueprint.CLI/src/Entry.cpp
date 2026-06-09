#include <print>
#include <iostream>

#include "Terminal.hpp"
#include "Services.hpp"

//
// Here will lie as follows:
// The Terminal Interface
//


int main()
{
	std::print("{}", Services::GetBlueprintsString());
} 