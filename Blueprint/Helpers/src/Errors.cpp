module;

module Helpers.Errors;

import std.compat;

std::unexpected<ErrorHandling::Error> MakeError(ErrorHandling::ErrorCode _Error, std::string _Message, std::source_location _Source)
{
    return std::unexpected(ErrorHandling::Error(std::move(_Error), std::move(_Message), _Source));
}
