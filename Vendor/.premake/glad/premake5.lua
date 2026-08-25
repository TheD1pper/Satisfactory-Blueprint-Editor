project "GLAD"
    kind "StaticLib"
    language "C"

    targetdir ("../../../build/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../../../build/bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "premake5.lua",
        "../../glad/include/glad/gl.h",
        "../../glad/include/KHR/khrplatform.h",
        "../../glad/src/gl.c"
    }

    includedirs { "../../glad/include" }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter {}
