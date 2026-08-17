module;

#include <print>
#include <queue>
#include <format>

#include "BenchmarkMacros.hpp"

module Parser.BinaryIO;

import <array>;

import Helpers.Benchmark;
import Helpers.Errors;
import Helpers.FsUtils;
import Core.Data;
import Core.Complex;
import Parser.Compression;

namespace Parser
{
	Result<Core::BlueprintHeader> InputBlueprint::ReadHeader()
	{
		// ==================================== Blueprint Header ==================================
		//  ┌────────┬───────────────────────────┬─────────┬─────────────────────────────────────┐
		//  │  Type  │           Field           │ Example │                Notes                │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ save header version       │ 14      │                                     │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ save version              │ 53      │                                     │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ build version             │ 418753  │                                     │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ size x                    │ 4       │ In 8 - meter units                  │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ size y                    │ 4       │ In 8 - meter units                  │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ size z                    │ 4       │ In 8 - meter units                  │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ number of cost entries    │ 11      │ number of entries                   │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ N / A  │ cost entries              │ N / A   │ Array of strings and uint32's       │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ uint32 │ number of content entries │ 18      │ number of entries                   │
		//	├────────┼───────────────────────────┼─────────┼─────────────────────────────────────┤
		//	│ N / A  │ content entries           │ N / A   │ Array of strings                    │
		//	└────────┴───────────────────────────┴─────────┴─────────────────────────────────────┘

		Core::BlueprintHeader Draft;
		{
			auto r_HeaderVersion = Read<uint32>();
			if (!r_HeaderVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad header version read"));
			if (*r_HeaderVersion != 2)
				return std::unexpected(Eh::Error(Eh::Blueprint::WrongHeaderVersion, "Blueprint format is outdated")); // This parser currently can read only version 2 blueprint file
			Draft.HeaderVersion = *r_HeaderVersion;

			auto r_SaveVersion = Read<uint32>();
			if (!r_SaveVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save version read"));
			Draft.SaveVersion = *r_SaveVersion;

			auto BuildVersion = Read<uint32>();
			if (!BuildVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad build version read"));
			Draft.BuildVersion = *BuildVersion;

			auto X = Read<uint32>();
			if (!X)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.X = *X;

			auto Y = Read<uint32>();
			if (!Y)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.Y = *Y;

			auto Z = Read<uint32>();
			if (!Z)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.Z = *Z;

			auto r_NumberOfCostEntries = Read<uint32>();
			if (!r_NumberOfCostEntries)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of cost entries read"));
			uint32 NumberOfCostEntries = *r_NumberOfCostEntries;
			if (NumberOfCostEntries <= 0)
				return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of cost entries can't be zero"));
			SkipBytes(4); // Ignore 4 empty bytes

			Draft.CostEntries.reserve(NumberOfCostEntries);
			for (uint32 i = 0; i < NumberOfCostEntries; i++)
			{
				auto r_String = Read<Core::String>();
				if (!r_String)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry class name read, iteration: " + static_cast<int>(i)));

				Core::CostEntry Entry;
				Entry.ClassName = *r_String;

				auto r_Quantity = Read<uint32>();
				if (!r_Quantity)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry quantity read"));
				Entry.Quantity = *r_Quantity;
				if (i != NumberOfCostEntries - 1)
					SkipBytes(4); // Ignore 4 empty bytes

				Draft.CostEntries.emplace_back(Entry);
			}

			auto r_NumberOfContentEntries = Read<uint32>();
			if (!r_NumberOfContentEntries)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of content entries read"));

			uint32 NumberOfContentEntries = *r_NumberOfContentEntries;
			if (NumberOfContentEntries <= 0)
				return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of content entries can't be zero"));
			SkipBytes(4); // Ignore 4 empty bytes

			Draft.ContentEntries.reserve(NumberOfContentEntries);
			for (uint32 i = 0; i < NumberOfContentEntries; i++)
			{
				Core::ContentEntry Entry;
				auto r_String = Read<Core::String>();
				if (!r_String)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad content entry class name read"));
				
				Entry.ClassName = *r_String;
				Draft.ContentEntries.emplace_back(Entry);
				if (i != NumberOfContentEntries - 1)
					SkipBytes(4);
			}

			// ==================================== Additional header ====================================
			// ┌────────┬────────────────────────────┬───────────────────────────────────────────┬───────┐
			// │  Type  │           Field            │                  Example                  │ Notes │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ uint32 │ save object data version   │ 0                                         │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ int    │ UE4 file version           │ 522                                       │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ int    │ UE5 file version           │ 1017                                      │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ int    │ licensee version           │ 3                                         │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ uint16 │ major engine version       │ 5                                         │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ uint16 │ minor engine version       │ 6                                         │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ uint16 │ engine patch version       │ 1                                         │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ uint32 │ engine version change list │ 2147971716                                │       │
			// ├────────┼────────────────────────────┼───────────────────────────────────────────┼───────┤
			// │ string │ engine version branch      │ ++FactoryGame+rel-main-1.2.0-experimental │       │
			// └────────┴────────────────────────────┴───────────────────────────────────────────┴───────┘
			// 
			// The documentation listed above is only valid when save version is equal or above 53

			if (Draft.SaveVersion >= 53)
			{
				auto r_SaveObjectDataVersion = Read<uint32>();
				if (!r_SaveObjectDataVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save object data version read"));
				Draft.SaveObjectDataVersion = *r_SaveObjectDataVersion;

				auto r_UE4FileVersion = Read<int>();
				if (!r_UE4FileVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad UE4 file version read"));
				Draft.PackageFileVersion.FileVersionUE4 = *r_UE4FileVersion;

				auto r_FileVersionUE5 = Read<int>();
				if (!r_FileVersionUE5)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad UE5 file version read"));
				Draft.PackageFileVersion.FileVersionUE5 = *r_FileVersionUE5;

				auto r_LicenseeVersion = Read<int>();
				if (!r_LicenseeVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad licensee version read"));
				Draft.LicenseeVersion = *r_LicenseeVersion;

				auto r_EngVerMajor = Read<uint16>();
				if (!r_EngVerMajor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad major engine version read"));
				Draft.EngineVersion.Major = *r_EngVerMajor;

				auto r_EngVerMinor = Read<uint16>();
				if (!r_EngVerMinor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad minor engine version read"));
				Draft.EngineVersion.Minor = *r_EngVerMinor;

				auto r_EngVerPatch = Read<uint16>();
				if (!r_EngVerPatch)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version patch read"));
				Draft.EngineVersion.Patch = *r_EngVerPatch;

				auto r_EngVerChangeList = Read<uint32>();
				if (!r_EngVerChangeList)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version change list read"));
				Draft.EngineVersion.ChangeList = *r_EngVerChangeList;

				auto r_EngVerBranch = Read<Core::String>();
				if (!r_EngVerBranch)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version branch read"));
				Draft.EngineVersion.Branch = *r_EngVerBranch;
			}
		}

		return Draft;
	}
	 
	Result<Core::BlueprintBody> InputBlueprint::ReadBody()
	{
		Core::BlueprintBody Draft;

		// =============================== Body chunk ===============================
		//  ┌────────┬─────────────────────────────────────┬───────────────────────┐
		//	│  Type  │                Field                │        Example        │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint32 │ UE package signature (constant)     │ 0x9E2A83C1            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint32 │ constant                            │ 0x22222222            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint32 │ max chunk size                      │ 0x00020000            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│  int8  │ padding                             │ 0x00                  │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint32 │ constant                            │ 0x03000000            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint64 │ compressed size                     │ 0xA90C0000            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint64 │ uncompressed size                   │ 0x00020000            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint64 │ compressed size (repeat)            │ 0xA90C0000            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ uint64 │ uncompressed size (repeat)          │ 0x00020000            │
		//	├────────┼─────────────────────────────────────┼───────────────────────┤
		//	│ N / A  │ bytes of uncompressed body          │ N / A                 │
		//  └────────┴─────────────────────────────────────┴───────────────────────┘
		// 
		// Between every blueprint header and body there's metadata that doesn't have a variable describing the size of it
		// Because of that you need to find a constant that you can "anchor" to to read the rest of the body
		// Every blueprint file has a couple of constants like 0x9E2A83C1, 0x22222222, 0x03000000, 0x00020100
		
		{
			BENCH_SCOPE("Anchor lookup");
			std::array<uint8, 4> SlidingWindow{ {0,0,0,0} };
			std::queue<uint8> Bytes;
			Result<uint8> Next{};

			while (true)
			{
				auto r_Next = Read<uint8>();
				if (!r_Next)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read byte when searching for body anchor"));

				uint8& Next = *r_Next;

				Bytes.push(Next);

				SlidingWindow[3] = SlidingWindow[2];
				SlidingWindow[2] = SlidingWindow[1];
				SlidingWindow[1] = SlidingWindow[0];
				SlidingWindow[0] = Next;

				if (SlidingWindow == std::array<uint8, 4>{0x9E, 0x2A, 0x83, 0xC1})
					break; 
			}

			if(SlidingWindow != std::array<uint8, 4>{0x9E, 0x2A, 0x83, 0xC1})
				return std::unexpected(Eh::Error(Eh::Blueprint::MissingBodyAnchor, "Could not find body anchor"));
		}


		Zlib ZCompressor;
		fs::path TempPath;
		BinaryOutput Temp;
		uint8 ChunkCount = 0;

		Temp.Close();

		while (true)
		{
			if (ChunkCount > 0)
			{
				auto r_SignatureCheck = Read<uint32>();
				if (!r_SignatureCheck or *r_SignatureCheck != UEPackageSignature)
					return std::unexpected(Eh::Error(Eh::Binary::CheckFailed, "Signature check went unsuccessful"));
			}

			auto r_ValidityCheck = Read<uint32>();
			if(!r_ValidityCheck or *r_ValidityCheck != FirstCheckValue)
				return std::unexpected(Eh::Error(Eh::Binary::CheckFailed, "First check went unsuccessful"));

			auto r_MaxChunkSize = Read<uint32>();
			if (!r_MaxChunkSize or *r_MaxChunkSize != MaxChunkSize)
				return std::unexpected(Eh::Error(Eh::Binary::CheckFailed, "Bad maximum chunk size (value need to be exactly 131 072)"));

			auto r_ValidityCheck2 = Read<uint8>();
			if (!r_ValidityCheck2 or *r_ValidityCheck2 != SecondCheckValue)
				return std::unexpected(Eh::Error(Eh::Binary::CheckFailed, "Second check went unsuccessful"));

			auto r_ValidityCheck3 = Read<uint32>();
			if (!r_ValidityCheck3 or *r_ValidityCheck3 != ThirdCheckValue)
				return std::unexpected(Eh::Error(Eh::Binary::CheckFailed, "Third check went unsuccessful"));

			auto r_CompressedSize = Read<uint64>();
			if (!r_CompressedSize)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad compressed size read"));

			auto r_UncompressedSize = Read<uint64>();
			if (!r_UncompressedSize)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad uncompressed size read"));

			SkipBytes(16); // Skip repeating sizes (16 bytes in total)

			Core::ByteVector Buffer(*r_CompressedSize);

			if (!ReadBytes(Buffer, *r_CompressedSize))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read compressed body"));
			Buffer.resize(*r_UncompressedSize);

			auto r_DecompressedBody = ZCompressor.Decompress(Buffer, *r_UncompressedSize);
			if (!r_DecompressedBody)
				return std::unexpected(Eh::Error(Eh::Compression::Fail, "Could not decompress body"));
			Buffer = std::move(*r_DecompressedBody);

			if (ChunkCount == 0)
			{
				TempPath = FsUtils::GetTempFolder() / std::format("Body-{}.tmp", *r_CompressedSize);
				Temp.Open(TempPath);
			}

			Temp.Write(Buffer);

			if (*r_UncompressedSize != MaxChunkSize) // Check if the chunk is full (if chunk equals max chunk size it problably isn't the last)
				break;

			ChunkCount++;
		}

		Temp.Close();
		if (!FsUtils::Destroy(TempPath))
			return std::unexpected(Eh::Error(Eh::FileIO::DeletionFail, "Could not delete temporary body"));

		return Draft;
	}	
}