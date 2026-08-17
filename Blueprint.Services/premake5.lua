project "Blueprint.Services"
    kind "StaticLib"
    language "C++"

    targetdir ("../bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../bin-int/%{cfg.buildcfg}/%{prj.name}")

    files
    {
        "modules/**.ixx",
        "src/**.cpp",
        "other/premake5.lua"
    }

    includedirs
    {
        "modules",
        "../Blueprint.Parser/modules",
        "../Blueprint.Core/modules",
        "../Blueprint.Helpers/modules",
        "../Blueprint.Helpers/include"
    }

    links
    {
        "Blueprint.Parser",
        "Blueprint.Core",
        "Blueprint.Helpers"
    }
