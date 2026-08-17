project "Core"
    kind "StaticLib"
    language "C++"

    targetdir ("../../bin/%{cfg.buildcfg}/Blueprint/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Blueprint/%{prj.name}")

    files {
        "modules/**.ixx",
        "src/**.cpp",
        "Core.lua"
    }

    includedirs {
        "modules",
        "../Helpers/modules",
        "../Helpers/include"
    }

    links {
        "Helpers"
    }
