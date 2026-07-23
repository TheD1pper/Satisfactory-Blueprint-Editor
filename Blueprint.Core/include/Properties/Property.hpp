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
        uint32_t Index{};

        Payload Value;
    };
}
