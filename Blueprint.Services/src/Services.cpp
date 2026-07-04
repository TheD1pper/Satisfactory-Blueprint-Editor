#include "Services.hpp"
#include "Parser.hpp"

#include <iostream>
#include <print>

namespace Services
{
	void ReadHeader()
	{
		fs::path fe = "C:\\Users\\Szymon\\AppData\\Local\\FactoryGame\\Saved\\SaveGames\\blueprints\\Exp 1.2\\Loop.sbp";
		Parser::InputBlueprint bp(fe);
		auto r = bp.ReadHeader();
		if (!r.has_value())
		{
			auto& err = r.error();
			
			if (err.Is<Eh::Binary>())
			{
				auto code = err.As<Eh::Binary>();
				switch (code)
				{
				case Eh::Binary::BadRead:
					std::cerr << err.GetLogMessage();
					break;

				case Eh::Binary::EmptyString:
					std::cerr << err.GetLogMessage();
					break;
				}
			}

			if (err.Is<Eh::Blueprint>())
			{
				auto code = err.As<Eh::Blueprint>();
				switch (code)
				{
				case Eh::Blueprint::WrongHeaderVersion:
					std::cerr << err.GetLogMessage();
					break;
				}
			}
		}
			
		r.value().Print();
		std::print("Bytes read: {}\n", bp.GetBytesRead());
	}
}