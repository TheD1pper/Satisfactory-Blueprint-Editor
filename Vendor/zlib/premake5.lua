project "zlib"
    kind "StaticLib"
    language "C"

    targetdir ("../../build/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../../build/bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "premake5.lua",
        "zconf.h",
        "zlib.h",
        "crc32.h",
        "deflate.h",
        "gzguts.h",
        "inffast.h",
        "inffixed.h",
        "inflate.h",
        "inftrees.h",
        "trees.h",
        "zutil.h",
        "adler32.c",
        "compress.c",
        "crc32.c",
        "deflate.c",
        "gzclose.c",
        "gzlib.c",
        "gzread.c",
        "gzwrite.c",
        "infback.c",
        "inffast.c",
        "inflate.c",
        "inftrees.c",
        "trees.c",
        "uncompr.c",
        "zutil.c"
    }

    includedirs { "." }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter {}
