module;

module Parser.BinaryIO;

import Helpers.Benchmark;
import Helpers.Errors;
import Helpers.FsUtils;
import Core.Data;
import Core.Complex;

namespace Parser
{
#pragma region Read implementations

	template <>
	Result<uint8_t> InputBlueprint::Read()
	{
		uint8_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 1))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Byte read"));
		return Value;
	}

	template <>
	Result<uint16_t> InputBlueprint::Read()
	{
		uint16_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 2))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint16 read"));
		return Value;
	}

	template <>
	Result<int> InputBlueprint::Read()
	{
		int Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Int read"));
		return Value;
	}

	template<>
	Result<float> InputBlueprint::Read()
	{
		float Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad float read"));
		return Value;
	}
	
	template <>
	Result<uint32_t> InputBlueprint::Read()
	{
		uint32_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 4))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint32 read"));
		return Value;
	}

	template <>
	Result<uint64_t> InputBlueprint::Read()
	{
		uint64_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 8))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Uint64 read"));
		return Value;
	}

	template <>
	Result<int64_t> InputBlueprint::Read()
	{
		int64_t Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 8))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad Int64 read"));
		return Value;
	}

	template <>
	Result<double> InputBlueprint::Read()
	{
		double Value{};

		if (!ReadBytes(reinterpret_cast<char*>(&Value), 8))
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad double read"));
		return Value;
	}

	template <>
	Result<Core::String> InputBlueprint::Read()
	{
		auto r_Length = Read<int>();

		if (!r_Length)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad string length read"));

		int SignedLength = r_Length.value();

		if (SignedLength == 0)
			return std::unexpected(Eh::Error(Eh::Binary::EmptyString, "Empty string"));

		Core::String Value{};

		if (SignedLength > 0) // UTF-8
		{
			Value.Utf = Core::Encoding::Utf8;
			uint32_t Length = static_cast<uint32_t>(SignedLength);

			Value.Content = std::string(Length - 1, '\0');

			if (!ReadBytes(Value.Content.data(), Length - 1))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad u8string read"));

			SkipBytes(1); // Ignore null-termination
			return Value;
		}
		else if (SignedLength < 0) // UTF-16
		{
			Value.Utf = Core::Encoding::Utf16;
			uint32_t ByteLength = static_cast<uint32_t>(-SignedLength);

			Value.Content = std::string(ByteLength - 2, '\0');

			if (!ReadBytes(Value.Content.data(), ByteLength - 2))
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad u16string read"));

			SkipBytes(2); // Ignore null-termination
			return Value;
		}

		return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad string length read"));
	}

	template<>
	Result<Core::Rotation4D> InputBlueprint::Read()
	{
		Core::Rotation4D Draft;

		for (size_t i = 0; i < 4; i++)
		{
			auto Rotation = Read<float>();
			if (!Rotation)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad rotation4D read"));

			switch (i)
			{
			case 0:
				Draft.X = *Rotation;
				break;
			case 1:
				Draft.Y = *Rotation;
				break;
			case 2:
				Draft.Z = *Rotation;
				break;
			case 3:
				Draft.W = *Rotation;
				break;
			}
		}

		return std::move(Draft);
	}

	template<>
	Result<Core::Position3D> InputBlueprint::Read()
	{
		Core::Position3D Draft;

		for (size_t i = 0; i < 3; i++)
		{
			auto Position = Read<float>();
			if (!Position)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad position3D read"));

			switch (i)
			{
			case 0:
				Draft.X = *Position;
				break;
			case 1:
				Draft.Y = *Position;
				break;
			case 2:
				Draft.Z = *Position;
				break;
			}
		}

		return std::move(Draft);
	}

	template<>
	Result<Core::Scale3D> InputBlueprint::Read()
	{
		Core::Scale3D Draft;

		for (size_t i = 0; i < 3; i++)
		{
			auto Scale = Read<float>();
			if (!Scale)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Bad scale3D read"));

			switch (i)
			{
			case 0:
				Draft.X = *Scale;
				break;
			case 1:
				Draft.Y = *Scale;
				break;
			case 2:
				Draft.Z = *Scale;
				break;
			}
		}

		return std::move(Draft);
	}

#pragma endregion Read() implementations of basic data types

#pragma region Complex implementations

	template<>
	Result<Core::ActorHeader> InputBlueprint::Read()
	{
		Core::ActorHeader Draft;

		auto r_TypePath = Read<Core::String>();
		if (!r_TypePath)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header type path"));
		Draft.TypePath = *r_TypePath;

		auto r_RootObject = Read<Core::String>();
		if (!r_RootObject)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header root object"));
		Draft.RootObject = *r_RootObject;

		auto r_InstanceName = Read<Core::String>();
		if (!r_InstanceName)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header instance name"));
		Draft.InstanceName = *r_InstanceName;

		auto r_Unknown = Read<uint32_t>();
		if (!r_Unknown)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header unknown"));
		Draft.Unknown = *r_Unknown;

		auto r_Rotation = Read<Core::Rotation4D>();
		if (!r_Rotation)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header rotation"));
		Draft.Rotation = *r_Rotation;

		auto r_Position = Read<Core::Position3D>();
		if (!r_Position)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header position"));
		Draft.Position = *r_Position;

		auto r_Scale = Read<Core::Scale3D>();
		if (!r_Scale)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header scale"));
		Draft.Scale = *r_Scale;
		
		auto r_NeedTransform = Read<uint32_t>();
		if (!r_NeedTransform)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header transform flag"));
		Draft.NeedTransform = *r_NeedTransform;

		auto r_IsPlaced = Read<uint32_t>();
		if (!r_IsPlaced)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header placed flag"));
		Draft.IsPlaced = *r_IsPlaced;

		return Draft;
	}

	template<>
	Result<Core::ComponentHeader> InputBlueprint::Read()
	{
		Core::ComponentHeader Draft;

		auto r_TypePath = Read<Core::String>();
		if (!r_TypePath)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read component header type path"));
		Draft.TypePath = *r_TypePath;

		auto r_RootObject = Read<Core::String>();
		if (!r_RootObject)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header type path"));
		Draft.RootObject = *r_RootObject;

		auto r_InstanceName = Read<Core::String>();
		if (!r_InstanceName)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header type path"));
		Draft.InstanceName = *r_InstanceName;

		auto r_Unknown = Read<uint32_t>();
		if (!r_Unknown)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header type path"));
		Draft.Unknown = *r_Unknown;

		auto r_ParentActorName = Read<Core::String>();
		if (!r_ParentActorName)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "could not read actor header type path"));
		Draft.ParentActorName = *r_ParentActorName;

		return std::move(Draft);
	}

	template<>
	Result<Core::ObjectHeader> InputBlueprint::Read()
	{
		Core::ObjectHeader Draft;

		auto r_Type = Read<uint32_t>();
		if (!r_Type)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read object type"));
		Draft.Type = static_cast<Core::ObjectHeaderType>(*r_Type);

		switch (*r_Type)
		{
		case 1:
		{
			Draft.Type = Core::ObjectHeaderType::Actor;
			auto r_ActorHeader = Read<Core::ActorHeader>();
			if (!r_ActorHeader)
				return std::unexpected(r_ActorHeader.error());
			Draft.Payload = std::move(*r_ActorHeader);
			return std::move(Draft);
		}
			break;


		case 0:
		{
			Draft.Type = Core::ObjectHeaderType::Component;
			auto r_ComponentHeader = Read<Core::ComponentHeader>();
			if (!r_ComponentHeader)
				return std::unexpected(r_ComponentHeader.error());
			Draft.Payload = std::move(*r_ComponentHeader);
			return std::move(Draft);
		}
			break;
		}

		return {};
	}

	template<>
	Result<Core::ObjectReference> InputBlueprint::Read()
	{
		Core::ObjectReference Draft;

		auto r_Level = Read<Core::String>();
		if (!r_Level)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read object reference level"));
		Draft.Level = *r_Level;

		auto r_Path = Read<Core::String>();
		if (!r_Path)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read object reference path"));
		Draft.Path = *r_Path;

		return Draft;
	}

	template<>
	Result<Core::ActorObject> InputBlueprint::Read()
	{
		Core::ActorObject Draft;

		auto r_Size = Read<uint32_t>();
		if(!r_Size)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read actor object size"));

		uint32_t& Size = *r_Size;

		auto r_ParentReference = Read<Core::ObjectReference>();
		if(!r_ParentReference)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read actor object parent reference"));

		Draft.ParentReference = *r_ParentReference;

		auto r_ComponentCount = Read<uint32_t>();
		if(!r_ComponentCount)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read actor object component count"));

		for (size_t i = 0; i < *r_ComponentCount; i++)
		{
			auto r_Component = Read<Core::ObjectReference>();
			if(!r_Component)
				return std::unexpected(Eh::Error(Eh::Binary::BadRead, std::format("Could not read actor object component (index: {})", i)));
			Draft.Components.push_back(std::move(*r_Component));
		}

		auto r_Properties = Read<Core::Property::PropertyList>();
		if (!r_Properties)
			return std::unexpected(Eh::Error(Eh::Binary::BadRead, "Could not read actor object property list"));

		SkipBytes(4); // Skip padding

		return Draft;
	}

#pragma endregion Read() implementations of complex data types

	// Property/PropertyList and every Payload type's Read<>() specialization live in
	// Property.cpp instead of here, to keep this file scoped to everything but property

#pragma region InputBlueprint
	uint64_t InputBlueprint::GetBytesRead() const
	{
		return BytesRead;
	}

	void InputBlueprint::SkipBytes(std::streamsize _Bytes)
	{
		BytesRead += _Bytes;
		Input.ignore(_Bytes);
	}

	std::istream& InputBlueprint::ReadBytes(char* _String, uint64_t _Count)
	{
		Input.read(_String, _Count);
		BytesRead += Input.gcount();
		return Input;
	}
#pragma endregion class funcions implementations

#pragma region OutputBlueprint
	fs::path InputBlueprint::GetPath()
	{
		return Path;
	}

	bool BinaryOutput::Write(std::string& _Data)
	{
		if(!Output.write(_Data.data(), _Data.size()))
			return false;

		BytesWritten += _Data.size();
		return true;
	}

	bool BinaryOutput::Write(Core::ByteVector _Data)
	{
		if(!Output.write(std::string(_Data.begin(), _Data.end()).c_str(), _Data.size()))
			return false;

		BytesWritten += _Data.size();
		return true;
	}

	inline std::streamsize BinaryOutput::GetBytesWritten() const
	{
		return BytesWritten;
	}

	InputBlueprint::InputBlueprint(const fs::path& _Path)
	{
		Path = _Path;
		Input = std::move(std::ifstream(Path, std::ios::binary));
	}

	BinaryOutput::BinaryOutput(fs::path _Path)
	{
		Output = std::ofstream(_Path, std::ios::binary);
	}
#pragma endregion class function implementations
}