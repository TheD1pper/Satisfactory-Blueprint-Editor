project "ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("../../build/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../../build/bin-int/%{cfg.buildcfg}/%{prj.name}")

    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imgui_demo.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",

        "backends/imgui_impl_glfw.h",
        "backends/imgui_impl_glfw.cpp",
        "backends/imgui_impl_opengl3.h",
        "backends/imgui_impl_opengl3.cpp",
        "backends/imgui_impl_opengl3_loader.h"
    }

    includedirs
    {
        ".",
        "../glfw/include"
    }

    filter "system:windows"
        systemversion "latest"

    filter {}
