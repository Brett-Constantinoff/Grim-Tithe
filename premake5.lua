workspace "Grim Tithe"
    configurations { "Debug", "Release" }
    startproject "Grim Tithe"
    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "DEBUG", "DEBUG_SHADER" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }

group "Game"
    include "src/grim-tithe.lua"

group "External"
    include "external/glfw.lua"
    include "external/glm.lua"