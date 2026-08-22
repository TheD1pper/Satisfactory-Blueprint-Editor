project "glm"
    kind "None"
    language "C++"

    files {
        "premake5.lua",
        "glm/**.hpp",
        "glm/**.h",
        "glm/**.inl"
    }

    includedirs { "." }
