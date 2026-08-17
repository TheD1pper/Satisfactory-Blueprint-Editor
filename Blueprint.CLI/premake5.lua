project "Blueprint.CLI"
    kind "ConsoleApp"
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
        "../Blueprint.Services/modules",
        "../Blueprint.Parser/modules",
        "../Blueprint.Core/modules",
        "../Blueprint.Helpers/modules",
        "../Blueprint.Helpers/include"
    }

    links
    {
        "Blueprint.Services",
        "Blueprint.Parser",
        "Blueprint.Core",
        "Blueprint.Helpers"
    }
