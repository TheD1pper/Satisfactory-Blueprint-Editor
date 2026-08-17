module;

export module Helpers.Errors;

import <string>;
import <expected>;
import <variant>;
import <source_location>;
import <type_traits>;

namespace ErrorHandling
{
    export template<typename T>
    concept EnumType = std::is_enum_v<T>;

    using Sl = std::source_location;

	export enum class FileIO
	{
		DeletionFail
	};

    export enum class Property
    {
        NotFound
    };

    export enum class Binary
    {
        BadWrite,
        BadRead,
        CantOpenFile,
        QuantityCantBeZero,
        EmptyString,
        CheckFailed
    };

    export enum class Blueprint
    {
        WrongHeaderVersion,
        MissingBodyAnchor
    };

    export enum class Compression
    {
        Fail
    };

    export using ErrorCode = std::variant<Binary, Blueprint, Compression, Property, FileIO>;

    export class Error
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

export template<typename T>
using Result = std::expected<T, ErrorHandling::Error>;

export namespace Eh = ErrorHandling;