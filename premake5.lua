workspace "Pistachio"
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
IncludeDir["GLFW"] = "%{wks.location}/Pistachio/vendor/GLFW/GLFW/include"
IncludeDir["glad"] = "%{wks.location}/Pistachio/vendor/glad/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Pistachio/vendor/imgui/ImGui"
IncludeDir["glm"] = "%{wks.location}/Pistachio/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Pistachio/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Pistachio/vendor/entt"
IncludeDir["json"] = "%{wks.location}/Pistachio/vendor/json/include"
IncludeDir["tinygltf"] = "%{wks.location}/PistachioShell/vendor/tinygltf"

project "Pistachio"
	location "Pistachio"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "Pistachio/src/pch.cpp"
	
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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.json}",
		"%{IncludeDir.tinygltf}"
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
		

include "Pistachio/vendor/GLFW"
include "Pistachio/vendor/glad"
include "Pistachio/vendor/imgui"

include "Samples/Counter"
include "PistachioShell"

