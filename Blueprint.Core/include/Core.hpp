#include <iostream>
#include <vector>

namespace Core
{
	class BinaryString
	{

	};

	class CostEntry
	{
		std::string ItemClass;
		uint32_t Quantity{};
	};

	class BlueprintHeader
	{
	public:
		uint32_t HeaderVersion{};
		uint32_t SaveVersion{};
		uint32_t BuildVersion{};
		uint32_t SizeX{};
		uint32_t SizeY{};
		uint32_t SizeZ{};
		uint32_t NumberOfCostEntries{};
		std::vector<CostEntry> CostEntries;
		uint32_t NumberOfContentEntries{};
		std::vector<std::string> ContentEntries;

	};
}