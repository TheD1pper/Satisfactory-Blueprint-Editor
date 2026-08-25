project "ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("../../../build/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../../../build/bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "premake5.lua",
        "../../imgui/imconfig.h",
        "../../imgui/imgui.h",
        "../../imgui/imgui.cpp",
        "../../imgui/imgui_draw.cpp",
        "../../imgui/imgui_internal.h",
        "../../imgui/imgui_tables.cpp",
        "../../imgui/imgui_widgets.cpp",
        "../../imgui/imgui_demo.cpp",
        "../../imgui/imstb_rectpack.h",
        "../../imgui/imstb_textedit.h",
        "../../imgui/imstb_truetype.h",

        "../../imgui/backends/imgui_impl_glfw.h",
        "../../imgui/backends/imgui_impl_glfw.cpp",
        "../../imgui/backends/imgui_impl_opengl3.h",
        "../../imgui/backends/imgui_impl_opengl3.cpp",
        "../../imgui/backends/imgui_impl_opengl3_loader.h"
    }

    includedirs {
        "../../imgui",
        "../../glfw/include"

    }

    filter "system:windows"
        systemversion "latest"

    filter {}
