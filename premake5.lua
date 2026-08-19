workspace "Satisfactory-Blueprint-Editor"
    architecture "x64"
    configurations {
        "Debug",
        "Release" ,
        "Distribution"
    }

    platforms {
        "Win64"
    }

    startproject "CLI"
    location "build/premake"
    characterset "MBCS"
    cppdialect "C++23"

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        defines "DISTRIBUTION"
        runtime "Release"
        optimize "On"

group "Solution Items"
    project "Solution Items"
        kind "None"
        files {
            ".gitignore",
            "premake5.lua",
            "README.md"
        }
group ""

group "Blueprint"
    include "Blueprint/CLI/CLI.lua"
    include "Blueprint/Services/Services.lua"
    include "Blueprint/Parser/Parser.lua"
    include "Blueprint/Core/Core.lua"
    include "Blueprint/Helpers/Helpers.lua"
    include "Blueprint/Tests/Tests.lua"
group ""

group "Vendor"
    include "Vendor/Catch2/Catch2.lua"
    include "Vendor/Zlib/Zlib.lua"
group ""