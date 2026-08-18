project "Zlib"
	kind "StaticLib"
	language "C"
	
    targetdir ("../../bin/%{cfg.buildcfg}/Vendor/%{prj.name}")
    objdir ("../../bin-int/%{cfg.buildcfg}/Vendor/%{prj.name}")

	files {
		"Zlib.lua",
		"*.h",
		"*.c"
	}

	includedirs {
		"."
	}

	buildoptions {
		"/Zc:preprocessor"
	}