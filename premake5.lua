workspace "Pistacio"
	architecture "x86_64"
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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}/target")
	objdir ("bin/" .. outputdir .. "/%{prj.name}/obj")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pistacio/vendor/spdlog/include",
		"Pistacio/src"
	}

	links
	{
		"Pistacio"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PSTC_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "PSTC_DEBUG"
		symbols "On"
		
	filter "configurations:VerboseDebug"
		defines {"PSTC_DEBUG", "PSTC_VERBOSE_DEBUG"}
		symbols "On"

	filter "configurations:Release"
		defines "PSTC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PSTC_DIST"
		symbols "On"


project "Pistacio"
	location "Pistacio"
	kind "SharedLib"
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
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
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
	
		postbuildcommands
		{
			("{MKDIR} ../bin/" .. outputdir .. "/Sandbox/target"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/target")
		}
		
	filter "configurations:Debug"
		defines "PSTC_DEBUG"
		symbols "On"

	filter "configurations:VerboseDebug"
		defines {"PSTC_DEBUG", "PSTC_VERBOSE_DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines "PSTC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PSTC_DIST"
		symbols "On"
		

include "Pistacio/vendor/GLFW"