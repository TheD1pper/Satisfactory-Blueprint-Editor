module;

export module Services.Manager;

import <memory>;
import <filesystem>;

import Helpers.Errors;
import Core.Data;
import Parser.BinaryIO;

namespace fs = std::filesystem;

namespace Services
{
	export class BlueprintManager
	{
	private:
		Core::Blueprint* m_Data = new Core::Blueprint[0];
		size_t m_Size{};
		size_t m_Capacity{};

	private:
		void ReAlloc(size_t _NewCapacity);
		void Reserve(size_t _Query);
		void PushBack(const Core::Blueprint& _Blueprint);

	public:
		size_t Size() const;
		Core::Blueprint& operator[](size_t _Index);
		const Core::Blueprint& operator[](size_t _Index) const;
		Core::Blueprint* Data();
		const Core::Blueprint* Data() const;

	public:
		[[nodiscard]]
		Result<void> Load(const fs::path& _Path);
		void Unload(size_t _Index);

	public:
		BlueprintManager();
		~BlueprintManager();
	};
}