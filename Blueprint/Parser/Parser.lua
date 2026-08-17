project "Parser"
    kind "StaticLib"
    language "C++"

    targetdir ("../../bin/%{cfg.buildcfg}/Blueprint/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Blueprint/%{prj.name}")

    files {
        "modules/**.ixx",
        "src/**.cpp",
        "Parser.lua"
    }

    includedirs {
        "modules",
        "../Core/modules",
        "../Helpers/modules",
        "../Helpers/include"
    }

    links {
        "Core",
        "Helpers"
    }
