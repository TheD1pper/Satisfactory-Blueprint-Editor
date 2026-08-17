project "CLI"
    kind "ConsoleApp"
    language "C++"

    targetdir ("../../bin/%{cfg.buildcfg}Blueprint//%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Blueprint/%{prj.name}")

    files {
        "modules/**.ixx",
        "src/**.cpp",
        "CLI.lua"
    }

    includedirs {
        "modules",
        "../Services/modules",
        "../Parser/modules",
        "../Core/modules",
        "../Helpers/modules",
        "../Helpers/include"
    }

    links {
        "Services",
        "Parser",
        "Core",
        "Helpers"
    }
