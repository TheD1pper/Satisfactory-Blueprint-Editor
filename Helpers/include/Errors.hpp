#pragma once

#include <string>
#include <expected>
#include <variant>
#include <source_location>
#include <type_traits>

namespace ErrorHandling
{
    template<typename T>
    concept EnumType = std::is_enum_v<T>;

    using Sl = std::source_location;

    enum class Binary
    {
        BadRead,
        QuantityCantBeZero,
        EmptyString,
        CheckFalied
    };

    enum class Blueprint
    {
        WrongHeaderVersion,
        MissingBodyAnchor
    };

    using ErrorCode = std::variant<Binary, Blueprint>;

    class Error
    {
    private:
        Sl Source;
        ErrorCode Code;
        std::string Message;

    public:
        const ErrorCode& GetCode() const { return Code; }
        const std::string& GetLogMessage() const { return Message; }

        template<EnumType T>
        explicit Error(T _ErrorCode, std::string _Message = "", Sl _Source = Sl::current())
            : Code(_ErrorCode),
            Message(std::move(_Message)),
            Source(_Source)
        {
        }

        template<typename T>
        bool Is() const
        {
            return std::holds_alternative<T>(Code);
        }

        template<typename T>
        const T* Get() const
        {
            return std::get_if<T>(&Code);
        }

        template<typename T>
        const T& As() const
        {
            return std::get<T>(Code);
        }

        Sl GetSource() const { return Source; }
    };
}

template<typename T>
using Result = std::expected<T, ErrorHandling::Error>;

namespace Eh = ErrorHandling;