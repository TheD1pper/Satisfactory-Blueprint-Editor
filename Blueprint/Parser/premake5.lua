project "Parser"
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
        "../Core/modules",
        "../Helpers/modules",

        -- Vendor includes
        "../../Vendor/zlib",
    }

    links {
        -- External links
        "Core",
        "Helpers",

        -- Vendor links
        "zlib",
    }