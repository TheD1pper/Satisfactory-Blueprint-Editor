project "Helpers"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"

    targetdir ("../../bin/%{cfg.buildcfg}/%{cfg.architecture}/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/%{cfg.architecture}/%{prj.name}")

    files {
        "premake5.lua",
        "src/**.cpp",
        "modules/**.ixx",
        "include/**.h",
    }