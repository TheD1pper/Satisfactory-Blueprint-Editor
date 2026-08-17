workspace "Satisfactory-Blueprint-Editor"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Blueprint.CLI"
    location "build/premake"
    characterset "MBCS"
    cppdialect "C++23"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter {}

include "Blueprint.CLI/CLI.lua"
include "Blueprint.Services/Services.lua"
include "Blueprint.Parser/Parser.lua"
include "Blueprint.Core/Core.lua"
include "Blueprint.Helpers/Helpers.lua"
