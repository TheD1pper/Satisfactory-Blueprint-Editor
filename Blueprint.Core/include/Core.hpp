#include <iostream>
#include <vector>

namespace Core
{
	// Basic data types
	// These are those that will be used to compose more advanced data structures

	enum class StringEncoding
	{
		Utf8, Utf16
	};

	class BinaryString
	{
	public:
		int Length{}; // Null-terminator included
		StringEncoding Encoding{};
		std::string Content;
	public:
		// If this function returns false you need to multiple the lenght by -2 to get the original string length
		bool isUtf8();
	};
	
	// Composite data types
	// These types require more information and space to work. Here defined types typically consist of multiple basic data types

	class CostEntry
	{
		BinaryString ItemClass;
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
		std::vector<BinaryString> ContentEntries;
	};
}