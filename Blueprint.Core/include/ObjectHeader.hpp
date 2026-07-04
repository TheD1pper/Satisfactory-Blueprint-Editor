#pragma once

#include <variant>

#include "ComponentHeader.hpp"
#include "ActorHeader.hpp"

namespace Core
{
	class ObjectHeader
	{
	private:
		std::variant<ActorHeader, ComponentHeader> Header;
	};
}
