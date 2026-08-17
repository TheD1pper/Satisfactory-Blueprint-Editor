project "Blueprint.Helpers"
    kind "StaticLib"
    language "C++"

    targetdir ("../bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../bin-int/%{cfg.buildcfg}/%{prj.name}")

    files
    {
        "modules/**.ixx",
        "src/**.cpp",
        "include/**.hpp",
        "other/premake5.lua"
    }

    includedirs
    {
        "modules",
        "include"
    }
