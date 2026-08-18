#include "catch_amalgamated.hpp"

int main(int argc, char* argv[])
{
	Catch::Session Test;
	Catch::ConfigData& Config = Test.configData();
	Config.showSuccessfulTests = true;

	return Test.run(argc, argv);
}