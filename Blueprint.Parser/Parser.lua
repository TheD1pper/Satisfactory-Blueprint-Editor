project "Blueprint.Parser"
    kind "StaticLib"
    language "C++"

    targetdir ("../bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../bin-int/%{cfg.buildcfg}/%{prj.name}")

    files
    {
        "modules/**.ixx",
        "src/**.cpp",
        "Parser.lua"
    }

    includedirs
    {
        "modules",
        "../Blueprint.Core/modules",
        "../Blueprint.Helpers/modules",
        "../Blueprint.Helpers/include"
    }

    links
    {
        "Blueprint.Core",
        "Blueprint.Helpers"
    }
