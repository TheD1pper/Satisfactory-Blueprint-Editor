#include "catch_amalgamated.hpp"

#include <print>

import <string>;

import Core.Data;
import Services.Manager;
import Helpers.FsUtils;

TEST_CASE("Blueprint manager succesfully reads blueprint")
{
	using namespace Services;
	using namespace Core;

	BlueprintManager Manager;
	fs::path ValidBlueprint = fs::current_path();
	ValidBlueprint = ValidBlueprint.parent_path();
	ValidBlueprint = ValidBlueprint.parent_path();
	ValidBlueprint = ValidBlueprint / "Blueprint\\Tests\\Data\\Valid\\ReinforcedPlateFactory.sbp";
	auto r_Load = Manager.Load(ValidBlueprint);

	SECTION("Load blueprint") {
		REQUIRE(r_Load.has_value() == true);
		REQUIRE(*r_Load == "ReinforcedPlateFactory");
	}


	Blueprint& Bp = Manager[*r_Load];

	SECTION("Blueprint is in the manager") {
		REQUIRE(Manager.Contains(*r_Load));
	}


	BlueprintHeader& Header = Bp.Header;
	BlueprintBody& Body = Bp.Body;

	SECTION("Blueprint can be used") {
		CHECK(Header.SaveVersion >= 53);
	}
}