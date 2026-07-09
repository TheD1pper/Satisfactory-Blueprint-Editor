#include "Services.hpp"
#include "Parser.hpp"

#include <iostream>
#include <print>

namespace Services
{
	std::unique_ptr<Core::Blueprint> LoadedBlueprint = std::make_unique<Core::Blueprint>();

	void LoadBlueprint(const fs::path& _Path)
	{
		Parser::InputBlueprint Input(_Path);

		auto r_Header = Input.ReadHeader();
		if (!r_Header.has_value())
		{
			auto& err = r_Header.error();

			if (err.Is<Eh::Binary>())
			{
				auto code = err.As<Eh::Binary>();
				switch (code)
				{
				case Eh::Binary::BadRead:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;

				case Eh::Binary::EmptyString:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;

				case Eh::Binary::QuantityCantBeZero:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;

				case Eh::Binary::CheckFalied:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;
				}
			}

			if (err.Is<Eh::Blueprint>())
			{
				auto code = err.As<Eh::Blueprint>();
				switch (code)
				{
				case Eh::Blueprint::WrongHeaderVersion:
					std::cerr << std::format("{} {} {}", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;
				}
			}
		}
		/*
		auto r_Body = Input.ReadBody();
		if (!r_Body.has_value() && r_Header.has_value())
		{
			auto& err = r_Body.error();

			if (err.Is<Eh::Binary>())
			{
				auto code = err.As<Eh::Binary>();
				switch (code)
				{
				case Eh::Binary::BadRead:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;

				case Eh::Binary::EmptyString:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;

				case Eh::Binary::QuantityCantBeZero:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;

				case Eh::Binary::CheckFalied:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;
				}
			}

			if (err.Is<Eh::Blueprint>())
			{
				auto code = err.As<Eh::Blueprint>();
				switch (code)
				{
				case Eh::Blueprint::WrongHeaderVersion:
					std::cerr << std::format("{} {} {}\n", err.GetLogMessage(), err.GetSource().file_name(), err.GetSource().line());
					break;
				}
			}
		} */

		if(r_Header.has_value())
			LoadedBlueprint->Header = *r_Header;
		//if(r_Body.has_value())
			//LoadedBlueprint->Body = *r_Body;
		
	}
}