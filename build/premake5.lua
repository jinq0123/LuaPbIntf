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
        "../luapbintf/**.lua",
        "../test/test.lua",
        "../test/*.proto",
    }
    includedirs {
        "../src",
        "../third_party/lua/src",
        "../third_party/lua-intf",
        "../third_party/protobuf/src",
    }
    libdirs {
        "../third_party/lib",
    }
    links {
        "lua",
    }

    --[[
    From: https://github.com/SteveKChiu/lua-intf
    By default LuaIntf expect the Lua library to build under C++.
    If you really want to use Lua library compiled under C,
    you can define LUAINTF_LINK_LUA_COMPILED_IN_CXX to 0:
    --]]
    -- defines { "LUAINTF_LINK_LUA_COMPILED_IN_CXX=0" }

    filter "configurations:Debug"
        flags { "Symbols" }
        libdirs { "../third_party/lib/Debug" }
        links { "libprotobufd" }
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        libdirs { "../third_party/lib/Release" }
        links { "libprotobuf" }
-- End of project "LuaPbIntf"
