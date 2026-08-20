project "Services"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"

    targetdir ("../../bin/%{cfg.buildcfg}/%{cfg.architecture}/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/%{cfg.architecture}/%{prj.name}")

    files {
        "premake5.lua",
        "src/**.cpp",
        "modules/**.ixx"
    }

     includedirs {
        -- Internal modules
        "modules",

        -- External modules
        "../Parser/modules",
        "../Core/modules",
        "../Helpers/modules",
    }

    links {
        -- External links
        "Parser",
        "Core",
        "Helpers",
    }
