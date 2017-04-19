-- premake5.lua
--[[
Usage examples: 
	for windows: premake5.exe --os=windows vs2015
	fot linux:   premake5.exe --os=linux gmake
]]

-- Todo: support non-vs2015, x64

workspace "LuaPbIntf"
	configurations { "Debug", "Release" }
project "LuaPbIntf"
	kind "SharedLib"
	targetname "luapbintf"
	language "C++"
	flags {
		"StaticRuntime",  -- for protobuf
	}
	files {
		"../src/**.h",
		"../src/**.cpp",
	}
	includedirs {
		"../src",
	}

	filter "configurations:Debug"
		flags { "Symbols" }
		links { "libprotobufd" }
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		links { "libprotobuf" }
-- End of project "LuaPbIntf"
