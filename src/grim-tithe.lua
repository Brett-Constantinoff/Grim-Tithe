local vulkan = os.getenv("VULKAN_SDK")

project "Grim Tithe"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    architecture "x64"

    targetdir "%{wks.location}/build/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/build/bin-int/%{cfg.buildcfg}"

    files 
    { 
        "**.cpp",
        "**.hpp",
        "**.h",
        "**.c",
    }

    includedirs { "grim-tithe/", "%{wks.location}/external/Glfw/include/", 
        "%{wks.location}/external/glm/", 
        vulkan .. "/Include" }
    links {vulkan .. "/Lib/vulkan-1.lib", "Glfw", "Glm"} 

    filter "configurations:Debug"
        defines { "DEBUG", "DEBUG_SHADER" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }