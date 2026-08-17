project "Services"
    kind "StaticLib"
    language "C++"

    targetdir ("../../bin/%{cfg.buildcfg}/Blueprint/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Blueprint/%{prj.name}")

    files {
        "modules/**.ixx",
        "src/**.cpp",
        "Services.lua"
    }

    includedirs {
        "modules",
        "../Parser/modules",
        "../Core/modules",
        "../Helpers/modules",
        "../Helpers/include"
    }

    links {
        "Parser",
        "Core",
        "Helpers"
    }
