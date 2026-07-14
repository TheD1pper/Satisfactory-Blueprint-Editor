#include <Windows.h>

#include "BasicDataTypes.hpp"

namespace Core
{
	std::string BinaryString::Utf16ToUtf8(std::u16string_view _String)
	{
        if (_String.empty())
            return {};

        int size = WideCharToMultiByte(
            CP_UTF8,
            0,
            reinterpret_cast<const wchar_t*>(_String.data()),
            static_cast<int>(_String.size()),
            nullptr,
            0,
            nullptr,
            nullptr);

        std::string Utf8(size, '\0');

        WideCharToMultiByte(
            CP_UTF8,
            0,
            reinterpret_cast<const wchar_t*>(_String.data()),
            static_cast<int>(_String.size()),
            Utf8.data(),
            size,
            nullptr,
            nullptr);

        return Utf8;
	}

    std::u16string BinaryString::Utf8ToUtf16(std::string_view _String)
    {
        if (_String.empty())
            return {};

        int size = MultiByteToWideChar(
            CP_UTF8,
            0,
            _String.data(),
            static_cast<int>(_String.size()),
            nullptr,
            0);

        std::u16string Utf16(size, u'\0');

        MultiByteToWideChar(
            CP_UTF8,
            0,
            _String.data(),
            static_cast<int>(_String.size()),
            reinterpret_cast<wchar_t*>(Utf16.data()),
            size);

        return Utf16;
    }
}