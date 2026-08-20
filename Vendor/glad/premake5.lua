project "GLAD"
    kind "StaticLib"
    language "C"

    targetdir ("../../build/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../../build/bin-int/%{cfg.buildcfg}/%{prj.name}")

    files
    {
        "include/glad/gl.h",
        "include/KHR/khrplatform.h",
        "src/gl.c"
    }

    includedirs { "include" }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter {}
