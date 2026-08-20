workspace "Satisfactory-Blueprint-Editor"
    architecture "x64"
    startproject "Blueprint Editor"
    location "build/premake"
    characterset "Unicode"
    cppdialect "C++23"

    configurations {
        "Debug",
        "Release",
        "Distribution"
    }

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        defines "SBE_DEBUG"

    filter "configurations:Release"
        symbols "On"
        optimize "Speed"
        defines "SBE_RELEASE"

    filter "configurations:Distribution"
        symbols "Off"
        optimize "Full"
        defines "SBE_DISTRIBUTION"

    filter "system:windows"
        systemversion "latest"
        defines "SBE_PLATFORM_WINDOWS"

    filter {}

include "Blueprint/Blueprint Editor/"
include "Blueprint/Services/"
include "Blueprint/Parser/"
include "Blueprint/Core/"
include "Blueprint/Helpers/"

group "Vendor"
    include "Vendor/glad/"
    include "Vendor/glfw/"
    include "Vendor/glm/"
    include "Vendor/imgui/"
    include "Vendor/zlib/"
group ""

group "Solution Items"
    project "Solution Items"
        kind "None"

        files {
            ".gitignore",
            "premake5.lua",
            "README.md",
            "LICENSE"
        }


group ""

