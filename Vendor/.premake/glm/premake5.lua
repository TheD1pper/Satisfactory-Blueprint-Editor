project "glm"
    kind "None"
    language "C++"

    files {
        "premake5.lua",
        "../../glm/glm/**.hpp",
        "../../glm/glm/**.h",
        "../../glm/glm/**.inl"
    }

    includedirs { "../../glm" }

    vpaths {
        ["Header Files"] = { "../../**.hpp", "../../**.h", "../../**.inl" },
        ["Premake Scripts"] = { "**.lua" }
    }
