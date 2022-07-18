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
IncludeDir["GLFW"] = "%{wks.location}/Pistacio/vendor/GLFW/GLFW/include"
IncludeDir["glad"] = "%{wks.location}/Pistacio/vendor/glad/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Pistacio/vendor/ImGui/ImGui"
IncludeDir["glm"] = "%{wks.location}/Pistacio/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Pistacio/vendor/stb_image"

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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
		
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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
include "Pistacio/vendor/tinygltf"

include "Samples/Counter"
include "Samples/Sandbox"

