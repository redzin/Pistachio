workspace "Pistacio"
	architecture "x86_64"
	startproject "CounterApp"
	configurations
	{
		"VerboseDebug",
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Pistacio/vendor/GLFW/GLFW/include"
IncludeDir["glad"] = "Pistacio/vendor/glad/glad/include"
IncludeDir["ImGui"] = "Pistacio/vendor/ImGui/ImGui"

project "Pistacio"
	location "Pistacio"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "Pistacio/src/pch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PSTC_PLATFORM_WINDOWS",
			"PSTC_BUILD_DLL"
		}
	
		
		
	filter "configurations:Debug"
		buildoptions "/MTd"
		defines "PSTC_DEBUG"
		symbols "On"

	filter "configurations:VerboseDebug"
		buildoptions "/MTd"
		defines {"PSTC_DEBUG", "PSTC_VERBOSE_DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		buildoptions "/MT"
		defines "PSTC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		buildoptions "/MT"
		defines "PSTC_DIST"
		symbols "On"
		

include "Pistacio/vendor/GLFW"
include "Pistacio/vendor/glad"
include "Pistacio/vendor/ImGui"

include "Samples/Counter"
include "Samples/Sandbox"

