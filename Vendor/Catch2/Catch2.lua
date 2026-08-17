project "Catch2"
    kind "StaticLib"
    language "C++"

    targetdir ("../bin/%{cfg.buildcfg}/Vendor/%{prj.name}")
    objdir ("../bin-int/%{cfg.buildcfg}/Vendor/%{prj.name}")

    files
    {
        "modules/**.ixx",
        "src/**.cpp",
        "include/*.hpp",
        "include/*.h",
        "Catch2.lua"
    }

    includedirs
    {
        "include",
        "modules"
    }
