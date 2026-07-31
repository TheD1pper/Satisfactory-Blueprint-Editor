module;

#include <cassert>
#include <iostream>

#include "BenchmarkMacros.hpp"

module Services.Manager;

import Helpers.Benchmark;
import Helpers.FsUtils;
import Helpers.Errors;
import Core.Data;
import Parser.BinaryIO;

namespace Services
{
	void BlueprintManager::ReAlloc(size_t _NewCapacity)
	{
		Core::Blueprint* NewData = new Core::Blueprint[_NewCapacity];

		if (_NewCapacity < m_Size)
			m_Size = _NewCapacity;

		for (size_t i = 0; i < m_Size; i++)
			NewData[i] = m_Data[i];

		delete[] m_Data;
		m_Data = NewData;
	}

	void BlueprintManager::Reserve(size_t _Query)
	{
		ReAlloc(m_Capacity + _Query);
		m_Capacity += _Query;
	}

	void BlueprintManager::PushBack(const Core::Blueprint& _Blueprint)
	{
		if (m_Capacity == m_Size)
			Reserve(1);

		m_Size++;
		m_Data[m_Size - 1] = _Blueprint;
	}

	size_t BlueprintManager::Size() const
	{
		return m_Size;
	}

	Core::Blueprint& BlueprintManager::operator[](size_t _Index)
	{
		assert(_Index < m_Size);
		return m_Data[_Index];
	}

	const Core::Blueprint& BlueprintManager::operator[](size_t _Index) const
	{
		assert(_Index < m_Size);
		return m_Data[_Index];
	}

	Core::Blueprint* BlueprintManager::Data()
	{
		return m_Data;
	}

	const Core::Blueprint* BlueprintManager::Data() const
	{
		return m_Data;
	}

	Result<void> BlueprintManager::Load(const fs::path& _Path)
	{
		Benchmark::Instrumentor::Get().BeginSession("Blueprint loading");
		BENCH_FUNC();

		Parser::InputBlueprint Input(_Path);
		Result<Core::BlueprintHeader> r_Header;
		{
			BENCH_SCOPE("Header read");
			r_Header = Input.ReadHeader();
			if (!r_Header)
			{
				return std::unexpected(r_Header.error());
			}
		}

		Result<Core::BlueprintBody> r_Body;
		{
			BENCH_SCOPE("Body read");
			r_Body = Input.ReadBody();
			if (!r_Body)
			{
				return std::unexpected(r_Body.error());
			}
		}

		{
			BENCH_SCOPE("Data assigment");
			PushBack({ *r_Header, *r_Body });
		}

		return {};
	}

	void BlueprintManager::Unload(size_t _Index)
	{
		m_Data[_Index] = {};
	}

	BlueprintManager::BlueprintManager()
	{
		Reserve(1);
	}

	BlueprintManager::~BlueprintManager()
	{
		delete[] m_Data;
	}
}