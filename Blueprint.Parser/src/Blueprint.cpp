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
import Core.Complex;
import Parser.Compression;

namespace Parser
{
	Result<Core::BlueprintHeader> InputBlueprint::ReadHeader()
	{
		Core::BlueprintHeader Draft;
		{
			auto r_HeaderVersion = Read<uint32_t>();
			if (!r_HeaderVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad header version read"));
			if (*r_HeaderVersion != 2)
				return std::unexpected(Eh::Error(Eh::Blueprint::WrongHeaderVersion, "Blueprint format is outdated")); // This parser currently can read only version 2 blueprint file
			Draft.HeaderVersion = *r_HeaderVersion;

			auto r_SaveVersion = Read<uint32_t>();
			if (!r_SaveVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save version read"));
			Draft.SaveVersion = *r_SaveVersion;

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
				auto r_SaveObjectDataVersion = Read<uint32_t>();
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

				auto r_EngVerMajor = Read<uint16_t>();
				if (!r_EngVerMajor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad major engine version read"));
				Draft.EngineVersion.Major = *r_EngVerMajor;

				auto r_EngVerMinor = Read<uint16_t>();
				if (!r_EngVerMinor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad minor engine version read"));
				Draft.EngineVersion.Minor = *r_EngVerMinor;

				auto r_EngVerPatch = Read<uint16_t>();
				if (!r_EngVerPatch)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version patch read"));
				Draft.EngineVersion.Patch = *r_EngVerPatch;

				auto r_EngVerChangeList = Read<uint32_t>();
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
				auto r_Next = Read<uint8_t>();
				if (!r_Next)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read byte when searching for body anchor"));
				uint8_t& Next = *r_Next;

				Bytes.push(Next);
				SlidingWindow[0] = SlidingWindow[1];
				SlidingWindow[1] = SlidingWindow[2];
				SlidingWindow[2] = SlidingWindow[3];
				SlidingWindow[3] = Next;
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

		auto r_MaximumChunkSize = Read<uint32_t>();
		if (!r_MaximumChunkSize || *r_MaximumChunkSize != MaxChunkSize)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Bad maximum chunk size (value need to be exactly 131 072)"));

		auto r_ValidityCheck2 = Read<uint8_t>();
		if (!r_ValidityCheck2 || *r_ValidityCheck2 != 0)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Second check went unsuccessful"));

		auto r_ValidityCheck3 = Read<uint32_t>();
		if (!r_ValidityCheck3 || *r_ValidityCheck3 != 0x03000000)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Third check went unsuccessful"));

		auto r_CompressedSize = Read<uint64_t>();
		if (!r_CompressedSize)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad compressed size read"));

		auto r_UncompressedSize = Read<uint64_t>();
		if (!r_UncompressedSize)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad uncompressed size read"));

		SkipBytes(16); // Skip data that repeats

		Core::ByteVector CompressedBody(*r_CompressedSize);
		{
			BENCH_SCOPE("Compressed body read");
			for (uint64_t i = 0; i < *r_CompressedSize; i++)
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
			r_UncompressedBody = Z.Decompress(std::move(CompressedBody), *r_UncompressedSize);
			if (!r_UncompressedBody)
				return std::unexpected(Eh::Error(Eh::Compression::Fail, "Could not decompress the body"));
		}

		fs::path TempPath;
		{
			Core::ByteVector UncompressedBody = *r_UncompressedBody;
			TempPath = FsUtils::GetTempFolder() / "Body.tmp";
			TempPath.replace_filename(std::format("Body-{}.tmp", *r_UncompressedSize));
			BinaryOutput TempOutput(TempPath);

			BENCH_SCOPE("Cache uncompressed body");
			if (!TempOutput.Write(UncompressedBody))
				return std::unexpected(Eh::Error(Eh::Binary::BadWrite, "Could not write temporary body"));
		}

		// Switch the files
		// Stop reading the finished file
		// Start reading the cached uncompressed body

		Input.close();
		Input.open(TempPath);
		BytesRead = 0;

		if (!Input.is_open())
			return std::unexpected(Eh::Error(Eh::Binary::CantOpenFile, "Could not open temporary uncompressed body"));

		{
			r_UncompressedSize = Read<uint32_t>();
			if (!r_UncompressedSize)
				return std::unexpected(r_UncompressedSize.error());

			BENCH_SCOPE("Object headers read");
			auto r_ObjectHeadersSize = Read<uint32_t>();
			if (!r_ObjectHeadersSize)
				return std::unexpected(r_ObjectHeadersSize.error());

			auto r_ObjectHeaderCount = Read<uint32_t>();
			if (!r_ObjectHeaderCount)
				return std::unexpected(r_ObjectHeaderCount.error());

			for (size_t i = 0; i < *r_ObjectHeaderCount; i++)
			{
				auto r_ObjectHeader = Read<Core::ObjectHeader>();
				if (!r_ObjectHeader)
					return std::unexpected(r_ObjectHeader.error());

				Draft.ObjectHeaders.emplace_back(std::move(*r_ObjectHeader));
			}
		}

		return Draft;
	}	
}