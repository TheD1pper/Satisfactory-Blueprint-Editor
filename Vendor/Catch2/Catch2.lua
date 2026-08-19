project "Catch2"
    kind "StaticLib"
    language "C++"

    defines "CATCH_AMALGAMATED_CUSTOM_MAIN"
   
    targetdir ("../../bin/%{cfg.buildcfg}/Vendor/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Vendor/%{prj.name}")

    files {
        "src/**.cpp",
        "include/*.hpp",
        "include/*.h",
        "Catch2.lua"
    }

    includedirs {
        "include",
        "modules"
    }
