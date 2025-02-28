project "Glm"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	architecture "x86_64"
	
	targetdir "%{wks.location}/build/external/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/build/external/bin-int/%{cfg.buildcfg}"

	includedirs { "glm/", "glm/glm" }

	files
	{
		"glm/glm/glm.cppm",
		"glm/glm/glm.hpp",
		"glm/glm/gtc/matrix_transform.hpp",
		"glm/glm/gtc/type_ptr.hpp",
		"glm/glm/gtc/quaternion.hpp",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"