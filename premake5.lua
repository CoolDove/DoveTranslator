workspace "DoveTranslator"
	configurations {"Debug", "Release"}
	platforms {"Linux", "Win"}
	language "C++"
	cppdialect "c++17"


project "dt"
	kind "ConsoleApp"
	location "dt/"
	targetdir "bin/%{cfg.platform}-%{cfg.buildcfg}"
	files {
		"dt/src/*.h",
		"dt/src/*.cpp"
	}
	-- configuration "Win"
	-- to be done
	configuration "Linux"
		links {
			"cppjson",
			"curl",
			"crypto"
		}
	includedirs {
		"cppjson/include"
	}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
	filter "configurations:Release"
		optimize "On"

project "cppjson"
	kind "StaticLib"
	location "cppjson"
	files {
		"cppjson/src/json/*.h",
		"cppjson/src/json/*.cpp"
	}
	includedirs {
		"cppjson/include"
	}
	targetdir "bin/%{cfg.platform}-%{cfg.buildcfg}"
