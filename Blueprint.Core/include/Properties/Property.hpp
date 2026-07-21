#pragma once

#include "PropertyEnums.hpp"
#include "BasicDataTypes.hpp"
#include "PropertyPayloads.hpp"

namespace Core::Property
{
    class Property
    {
    public:
        String Name;
        PropertyType Type{};
        Uint32 Index{};

        Payload Value;
    };
}
