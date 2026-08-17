project "Helpers"
    kind "StaticLib"
    language "C++"

    targetdir ("../../bin/%{cfg.buildcfg}/Blueprint/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Blueprint/%{prj.name}")

    files {
        "modules/**.ixx",
        "src/**.cpp",
        "include/**.hpp",
        "Helpers.lua"
    }

    includedirs {
        "modules",
        "include"
    }
