project "Blueprint.Core"
    kind "StaticLib"
    language "C++"

    targetdir ("../bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../bin-int/%{cfg.buildcfg}/%{prj.name}")

    files
    {
        "modules/**.ixx",
        "src/**.cpp",
        "premake5.lua"
    }

    includedirs
    {
        "modules",
        "../Blueprint.Helpers/modules",
        "../Blueprint.Helpers/include"
    }

    links
    {
        "Blueprint.Helpers"
    }
