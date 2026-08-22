project "Blueprint Editor"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++23"

    filter "system:windows"
        enablemodules "On"
        buildstlmodules "On"
    filter {}

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
        "../Services/modules",
        "../Parser/modules",
        "../Core/modules",
        "../Helpers/modules",

        -- Vendor includes
        "../../Vendor/glad/include",
        "../../Vendor/glfw/include",
        "../../Vendor/glm",
        "../../Vendor/imgui",
        "../../Vendor/imgui/backends"
    }

    links {
        -- External links
        "Services",
        "Parser",
        "Core",
        "Helpers",

        -- Vendor links
        "GLAD",
        "GLFW",
        "GLM",
        "ImGui"
    }