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

include "Blueprint.Helpers/premake5.lua"
include "Blueprint.Core/premake5.lua"
include "Blueprint.Parser/premake5.lua"
include "Blueprint.Services/premake5.lua"
include "Blueprint.CLI/premake5.lua"
