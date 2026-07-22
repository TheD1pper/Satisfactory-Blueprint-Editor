#include <print>
#include <array>
#include <queue>
#include <iostream>

#include "Parser.hpp"
#include "BasicDataTypes.hpp"
#include "Benchmark.hpp"

namespace Parser
{	
	Result<Core::BlueprintHeader> InputBlueprint::ReadHeader()
	{
		Core::BlueprintHeader Draft;
		{
			auto HeaderVersion = Read<Core::Uint32>();
			if (!HeaderVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad header version read"));
			if (*HeaderVersion != 2)
				return std::unexpected(Eh::Error(Eh::Blueprint::WrongHeaderVersion, "Blueprint format is outdated")); // This parser currently can read only version 2 blueprint file
			Draft.HeaderVersion = *HeaderVersion;

			auto SaveVersion = Read<Core::Uint32>();
			if (!SaveVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save version read"));
			Draft.SaveVersion = *SaveVersion;

			auto BuildVersion = Read<Core::Uint32>();
			if (!BuildVersion)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad build version read"));
			Draft.BuildVersion = *BuildVersion;

			auto X = Read<Core::Uint32>();
			if (!X)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.X = *X;

			auto Y = Read<Core::Uint32>();
			if (!Y)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.Y = *Y;

			auto Z = Read<Core::Uint32>();
			if (!Z)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad size X read"));
			Draft.Size.Z = *Z;

			auto r_NumberOfCostEntries = Read<Core::Uint32>();
			if (!r_NumberOfCostEntries)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of cost entries read"));
			Core::Uint32 NumberOfCostEntries = *r_NumberOfCostEntries;
			if (NumberOfCostEntries <= 0)
				return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of cost entries can't be zero"));
			SkipBytes(4); // Ignore 4 empty bytes

			std::vector<Core::CostEntry> CostEntries;
			CostEntries.reserve(NumberOfCostEntries);
			for (uint32_t i = 0; i < NumberOfCostEntries; i++)
			{
				auto r_String = Read<Core::String>();
				if (!r_String)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry class name read, iteration: " + static_cast<int>(i)));

				Core::CostEntry Draft;
				Draft.ClassName = *r_String;

				auto r_Quantity = Read<Core::Uint32>();
				if (!r_Quantity)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad cost entry quantity read"));
				Draft.Quantity = *r_Quantity;
				if (i != NumberOfCostEntries - 1)
					SkipBytes(4); // Ignore 4 empty bytes

				CostEntries.emplace_back(Draft);
			}

			Draft.CostEntries = std::move(CostEntries);

			auto r_NumberOfContentEntries = Read<Core::Uint32>();
			if (!r_NumberOfContentEntries)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad number of content entries read"));

			Core::Uint32 NumberOfContentEntries = *r_NumberOfContentEntries;
			if (NumberOfContentEntries <= 0)
				return std::unexpected(Eh::Error(Eh::Binary::QuantityCantBeZero, "Number of content entries can't be zero"));
			SkipBytes(4); // Ignore 4 empty bytes

			std::vector<Core::ContentEntry> ContentEntries;
			ContentEntries.reserve(NumberOfContentEntries);

			for (uint32_t i = 0; i < NumberOfContentEntries; i++)
			{
				Core::ContentEntry Draft;
				auto r_String = Read<Core::String>();
				if (!r_String)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad content entry class name read"));
				Draft.ClassName = *r_String;
				ContentEntries.emplace_back(Draft);
				if (i != NumberOfContentEntries - 1)
					SkipBytes(4);
			}
			Draft.ContentEntries = std::move(ContentEntries);

			if (Draft.SaveVersion >= 53)
			{
				auto SaveObjectDataVersion = Read<Core::Uint32>();
				if (!SaveObjectDataVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad save object data version read"));
				Draft.SaveObjectDataVersion = *SaveObjectDataVersion;

				auto UE4FileVersion = Read<Core::Int>();
				if (!UE4FileVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad UE4 file version read"));
				Draft.PackageFileVersion.FileVersionUE4 = *UE4FileVersion;

				auto FileVersionUE5 = Read<Core::Int>();
				if (!FileVersionUE5)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad UE5 file version read"));
				Draft.PackageFileVersion.FileVersionUE5 = *FileVersionUE5;

				auto LicenseeVersion = Read<Core::Int>();
				if (!LicenseeVersion)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad licensee version read"));
				Draft.LicenseeVersion = *LicenseeVersion;

				auto EngVerMajor = Read<Core::Uint16>();
				if (!EngVerMajor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad major engine version read"));
				Draft.EngineVersion.Major = *EngVerMajor;

				auto EngVerMinor = Read<Core::Uint16>();
				if (!EngVerMinor)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad minor engine version read"));
				Draft.EngineVersion.Minor = *EngVerMinor;

				auto EngVerPatch = Read<Core::Uint16>();
				if (!EngVerPatch)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad engine version patch read"));
				Draft.EngineVersion.Patch = *EngVerPatch;

				auto EngVerChangeList = Read<Core::Uint32>();
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
			BENCH_SCOPE("Anchor lookout");
			std::array<Core::Byte, 4> SlidingWindow{ {0,0,0,0} };
			std::queue<Core::Byte> Bytes;
			Result<Core::Byte> Next{};

			while (SlidingWindow != std::array<Core::Byte, 4>{0x22, 0x22, 0x22, 0x22})
			{
				auto Next = Read<Core::Byte>();

				if (!Next)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read byte when searching for body anchor"));
				
				Bytes.push(*Next);
				SlidingWindow[0] = SlidingWindow[1];
				SlidingWindow[1] = SlidingWindow[2];
				SlidingWindow[2] = SlidingWindow[3];
				SlidingWindow[3] = *Next;
			}

			if (SlidingWindow != std::array<Core::Byte, 4>{0x22, 0x22, 0x22, 0x22})
				return std::unexpected(Eh::Error(Eh::Blueprint::MissingBodyAnchor, "Could not find body anchor"));

			for (int i = 0; i < SlidingWindow.size() + 4; i++) // Pop the constant bytes in the queue, they are not needed for saving
				Bytes.pop();

			for (int i = 0; i < 4; i++) // cast the array into one variable
			{
				Draft.UEPackageSignature |= static_cast<Core::Uint32>(Bytes.front()) << (8 * i);
				Bytes.pop();
			}
			AnchorAddress = Input.tellg();
		}
		auto MaximumChunkSize = Read<Core::Uint32>();
		if (!MaximumChunkSize || MaximumChunkSize != MaxChunkSize)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Bad maximum chunk size (value need to be exactly 131 072)"));

		auto ValidityCheck2 = Read<Core::Byte>();
		if (!ValidityCheck2 || *ValidityCheck2 != 0)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Second check went unsuccessful"));

		auto ValidityCheck3 = Read<Core::Uint32>();
		if (!ValidityCheck3 || *ValidityCheck3 != 0x03000000)
			return std::unexpected(Eh::Error(Eh::Binary::CheckFalied, "Third check went unsuccessful"));

		for (int i = 0; i < 2; i++) // Loop needs to be iterated 2 times because the data repeats
		{
			auto ComprossedSize = Read<Core::Uint64>();
			if (!ComprossedSize)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad compressed size read"));
			Draft.CompressedSize = *ComprossedSize;

			auto UncompressedSize = Read<Core::Uint64>();
			if (!UncompressedSize)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad uncompressed size read"));
			Draft.UncompressedSize = *UncompressedSize;
			Draft.UncompressedSize = *UncompressedSize;
		}

			ByteVector CompressedBody(Draft.CompressedSize);
		{
			BENCH_SCOPE("Compressed body read");
			for (uint64_t i = 0; i < Draft.CompressedSize; i++)
			{
				auto byte = Read<Core::Byte>();
				if (!byte)
					return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read one of the compressed body bytes " + GetBytesRead()));
				CompressedBody[i] = *byte;
			}
		}

		Result<Core::ByteVector> r_UncompressedBody;
		{
			BENCH_SCOPE("Decompression");
			r_UncompressedBody = Z.Decompress(std::move(CompressedBody), Draft.UncompressedSize);
			if (!r_UncompressedBody)
				return std::unexpected(Eh::Error(Eh::Compression::Fail, r_UncompressedBody.error().GetLogMessage()));
		}
		
		ByteVector UncompressedBody = *r_UncompressedBody;
		return Draft;
	}	


}