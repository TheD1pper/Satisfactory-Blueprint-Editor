#include "Core.hpp"

//
// Here will lie as follows:
// Data Types
// Blueprint Models
//

namespace Core
{
	bool BinaryString::isUtf8()
	{
		if (Length > 0)
			return true;
		return false;
	}
}