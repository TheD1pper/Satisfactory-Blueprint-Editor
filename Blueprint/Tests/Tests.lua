project "Tests"
    kind "ConsoleApp"
    language "C++"

    targetdir ("../../bin/%{cfg.buildcfg}/Blueprint/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Blueprint/%{prj.name}")

    files {
        "*.cpp",
        "*.hpp",
        "**/*.cpp",
        "**/.hpp",
        "README.md",
        "Tests.lua"
    }

    includedirs {
        "../../Vendor/Catch2/include",
        "../CLI/modules",
        "../Services/modules",
        "../Parser/modules",
        "../Core/modules",
        "../Helpers/modules",
        "../Helpers/include"
    }

    links {
        "Catch2",
        "Services",
        "Parser",
        "Core",
        "Helpers"
    }
