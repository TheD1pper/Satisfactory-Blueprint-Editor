#include <print>

#include "Core.hpp"

namespace Core
{
	void BlueprintHeader::Print()
	{
		std::print("Header version: {}\n", HeaderVersion);
		std::print("Save version: {}\n", SaveVersion);
		std::print("Build version: {}\n", BuildVersion);
		std::print("X: {}, Y: {}, Z: {}\n", Size.X, Size.Y, Size.Z);

		std::print("======Cost Entries======\n");

		for (auto& Entry : CostEntries)
			std::print("Class name: {} \n",Entry.ClassName.Content);

		std::print("======Content Entries======\n");

		for (auto& Entry : ContentEntries)
			std::print("Class Name: {}\n", Entry.ClassName.Content);
	}
}