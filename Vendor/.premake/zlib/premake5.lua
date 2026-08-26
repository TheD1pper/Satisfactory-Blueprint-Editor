project "zlib"
    kind "StaticLib"
    language "C"

    targetdir ("../../../build/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../../../build/bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "premake5.lua",
        "../../zlib/zconf.h",
        "../../zlib/zlib.h",
        "../../zlib/crc32.h",
        "../../zlib/deflate.h",
        "../../zlib/gzguts.h",
        "../../zlib/inffast.h",
        "../../zlib/inffixed.h",
        "../../zlib/inflate.h",
        "../../zlib/inftrees.h",
        "../../zlib/trees.h",
        "../../zlib/zutil.h",
        "../../zlib/adler32.c",
        "../../zlib/compress.c",
        "../../zlib/crc32.c",
        "../../zlib/deflate.c",
        "../../zlib/gzclose.c",
        "../../zlib/gzlib.c",
        "../../zlib/gzread.c",
        "../../zlib/gzwrite.c",
        "../../zlib/infback.c",
        "../../zlib/inffast.c",
        "../../zlib/inflate.c",
        "../../zlib/inftrees.c",
        "../../zlib/trees.c",
        "../../zlib/uncompr.c",
        "../../zlib/zutil.c"
    }

    includedirs { "../../zlib" }

    vpaths {
        ["Header Files"] = { "../../**.h" },
        ["Source Files"] = { "../../**.c" },
        ["Premake Scripts"] = { "**.lua" }
    }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter {}
