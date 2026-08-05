module;

#include <print>
#include <queue>
#include <format>

#include "BenchmarkMacros.hpp"

module Parser.BinaryIO;

import <array>;
import <cstdint>;

import Helpers.Benchmark;
import Helpers.Errors;
import Helpers.FsUtils;
import Core.Data;
import Parser.Compression;

namespace Parser
{	
	Result<Core::BlueprintHeader> InputBlueprint::ReadHeader()
	{
		Core::BlueprintHeader Draft;
		{
			auto HeaderVersion = Read<uint32_t>();
			if (!HeaderVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad header version read"));
			if (*HeaderVersion != 2)
				return std::unexpected(Eh::Error(Eh::Blueprint::WrongHeaderVersion, "Blueprint format is outdated")); // This parser currently can read only version 2 blueprint file
			Draft.HeaderVersion = *HeaderVersion;

			auto SaveVersion = Read<uint32_t>();
			if (!SaveVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save version read"));
			Draft.SaveVersion = *SaveVersion;

			auto BuildVersion = Read<uint32_t>();
			if (!BuildVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad build version read"));
			Draft.BuildVersion = *BuildVersion;

			auto X = Read<uint32_t>();
			if (!X)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.X = *X;

			auto Y = Read<uint32_t>();
			if (!Y)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.Y = *Y;

			auto Z = Read<uint32_t>();
			if (!Z)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.Z = *Z;

			auto r_NumberOfCostEntries = Read<uint32_t>();
			if (!r_NumberOfCostEntries)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of cost entries read"));
			uint32_t NumberOfCostEntries = *r_NumberOfCostEntries;
			if (NumberOfCostEntries <= 0)
				return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of cost entries can't be zero"));
			SkipBytes(4); // Ignore 4 empty bytes

			Draft.CostEntries.reserve(NumberOfCostEntries);
			for (uint32_t i = 0; i < NumberOfCostEntries; i++)
			{
				auto r_String = Read<Core::String>();
				if (!r_String)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry class name read, iteration: " + static_cast<int>(i)));

				Core::CostEntry Entry;
				Entry.ClassName = *r_String;

				auto r_Quantity = Read<uint32_t>();
				if (!r_Quantity)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry quantity read"));
				Entry.Quantity = *r_Quantity;
				if (i != NumberOfCostEntries - 1)
					SkipBytes(4); // Ignore 4 empty bytes

				Draft.CostEntries.emplace_back(Entry);
			}

			auto r_NumberOfContentEntries = Read<uint32_t>();
			if (!r_NumberOfContentEntries)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of content entries read"));

			uint32_t NumberOfContentEntries = *r_NumberOfContentEntries;
			if (NumberOfContentEntries <= 0)
				return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of content entries can't be zero"));
			SkipBytes(4); // Ignore 4 empty bytes

			Draft.ContentEntries.reserve(NumberOfContentEntries);
			for (uint32_t i = 0; i < NumberOfContentEntries; i++)
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

			if (Draft.SaveVersion >= 53)
			{
				auto SaveObjectDataVersion = Read<uint32_t>();
				if (!SaveObjectDataVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save object data version read"));
				Draft.SaveObjectDataVersion = *SaveObjectDataVersion;

				auto UE4FileVersion = Read<int>();
				if (!UE4FileVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad UE4 file version read"));
				Draft.PackageFileVersion.FileVersionUE4 = *UE4FileVersion;

				auto FileVersionUE5 = Read<int>();
				if (!FileVersionUE5)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad UE5 file version read"));
				Draft.PackageFileVersion.FileVersionUE5 = *FileVersionUE5;

				auto LicenseeVersion = Read<int>();
				if (!LicenseeVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad licensee version read"));
				Draft.LicenseeVersion = *LicenseeVersion;

				auto EngVerMajor = Read<uint16_t>();
				if (!EngVerMajor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad major engine version read"));
				Draft.EngineVersion.Major = *EngVerMajor;

				auto EngVerMinor = Read<uint16_t>();
				if (!EngVerMinor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad minor engine version read"));
				Draft.EngineVersion.Minor = *EngVerMinor;

				auto EngVerPatch = Read<uint16_t>();
				if (!EngVerPatch)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version patch read"));
				Draft.EngineVersion.Patch = *EngVerPatch;

				auto EngVerChangeList = Read<uint32_t>();
				if (!EngVerChangeList)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version change list read"));
				Draft.EngineVersion.ChangeList = *EngVerChangeList;

				auto EngVerBranch = Read<Core::String>();
				if (!EngVerBranch)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version branch read"));
				Draft.EngineVersion.Branch = *EngVerBranch;
			}
		}

		return Draft;
	}
	 
	Result<Core::BlueprintBody> InputBlueprint::ReadBody()
	{
		Core::BlueprintBody Draft;

		std::streamoff AnchorAddress{}; 
		{
			// Between every blueprint header and body there's metadata that doesn't have a variable describing the size of it
			// Because of that you need to find a constant that you can "anchor" to to read the rest of the body
			// Every blueprint file has a couple of constants like 0x22222222, 0x03000000, 0x00020100
			// Only the first anchor is looked up, a failsafe can be added to limit the probability of the anchor recurring 

			BENCH_SCOPE("Anchor lookup");
			std::array<uint8_t, 4> SlidingWindow{ {0,0,0,0} };
			std::queue<uint8_t> Bytes;
			Result<uint8_t> Next{};

			while (SlidingWindow != std::array<uint8_t, 4>{0x22, 0x22, 0x22, 0x22})
			{
				auto Next = Read<uint8_t>();

				if (!Next)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read byte when searching for body anchor"));
				
				Bytes.push(*Next);
				SlidingWindow[0] = SlidingWindow[1];
				SlidingWindow[1] = SlidingWindow[2];
				SlidingWindow[2] = SlidingWindow[3];
				SlidingWindow[3] = *Next;
			}

			if (SlidingWindow != std::array<uint8_t, 4>{0x22, 0x22, 0x22, 0x22})
				return std::unexpected(Eh::Error(Eh::Blueprint::MissingBodyAnchor, "Could not find body anchor"));

			for (int i = 0; i < SlidingWindow.size() + 4; i++) // Pop the constant bytes in the queue, they are not needed for saving
				Bytes.pop();

			for (int i = 0; i < 4; i++) // cast the array into one variable
			{
				Draft.UEPackageSignature |= static_cast<uint32_t>(Bytes.front()) << (8 * i);
				Bytes.pop();
			}
			AnchorAddress = Input.tellg();
		}
		auto MaximumChunkSize = Read<uint32_t>();
		if (!MaximumChunkSize || MaximumChunkSize != MaxChunkSize)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Bad maximum chunk size (value need to be exactly 131 072)"));

		auto ValidityCheck2 = Read<uint8_t>();
		if (!ValidityCheck2 || *ValidityCheck2 != 0)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Second check went unsuccessful"));

		auto ValidityCheck3 = Read<uint32_t>();
		if (!ValidityCheck3 || *ValidityCheck3 != 0x03000000)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Third check went unsuccessful"));

		auto CompressedSize = Read<uint64_t>();
		if (!CompressedSize)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad compressed size read"));

		auto UncompressedSize = Read<uint64_t>();
		if (!UncompressedSize)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad uncompressed size read"));

		SkipBytes(16); // Skip data that repeats

		Core::ByteVector CompressedBody(*CompressedSize);
		{
			BENCH_SCOPE("Compressed body read");
			for (uint64_t i = 0; i < *CompressedSize; i++)
			{
				auto byte = Read<uint8_t>();
				if (!byte)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read one of the compressed body bytes " + GetBytesRead()));
				CompressedBody[i] = *byte;
			}
		}

		Result<Core::ByteVector> r_UncompressedBody;
		{
			Zlib Z;
			BENCH_SCOPE("Decompression");
			r_UncompressedBody = Z.Decompress(std::move(CompressedBody), *UncompressedSize);
			if (!r_UncompressedBody)
				return std::unexpected(Eh::Error(Eh::Compression::Fail, r_UncompressedBody.error().GetLogMessage()));
		}

		fs::path TempPath;
		{
			Core::ByteVector UncompressedBody = *r_UncompressedBody;
			TempPath = FsUtils::GetTempFolder() / "Body.tmp";
			BinaryOutput TempOutput(TempPath);
			TempPath.replace_filename(std::format("Body-{}.tmp", *UncompressedSize));
		
			BENCH_SCOPE("Cache uncompressed body");
			if (!TempOutput.Write(UncompressedBody))
				return std::unexpected(Eh::Error(Eh::Binary::BadWrite, "Could not write temporary body"));
		}

		// Switch the files
		// Stop reading the finished file
		// Start reading the cached uncompressed body

		Input.close();
		Input.open(TempPath);

		if (!Input.is_open())
			std::print("something went wrong\n");

		BENCH_SCOPE("Object headers read");
		{
			UncompressedSize = Read<uint32_t>();
			if (!UncompressedSize)
				return std::unexpected(UncompressedSize.error());

			auto ObjectHeadersSize = Read<uint32_t>();
			if (!ObjectHeadersSize)
				return std::unexpected(ObjectHeadersSize.error());

			auto CountOfObjectHeaders = Read<uint32_t>();
			if (!CountOfObjectHeaders)
				return std::unexpected(CountOfObjectHeaders.error());

			for (size_t i = 0; i < *CountOfObjectHeaders; i++)
			{
				auto ObjectHeader = Read<Core::ObjectHeader>();
				if (!ObjectHeader)
					return std::unexpected(ObjectHeader.error());

				Draft.ObjectHeaders.push_back(std::move(*ObjectHeader));
			}
		}

		return Draft;
	}	
}