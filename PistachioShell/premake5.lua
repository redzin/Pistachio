project "Pistachio Shell"
	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/target")
	objdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/obj")
	
	files
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	includedirs
	{
		"%{wks.location}/Pistachio/vendor/spdlog/include",
		"%{wks.location}/Pistachio/src",
		"%{IncludeDir.glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.json}",
		"%{IncludeDir.tinygltf}"
	}

	links
	{
		"Pistachio"
	}
	
	defines
	{
		"PSTC_PLATFORM_WINDOWS",
		"PSTC_BUILD_DLL"
	}
	
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		
	filter "configurations:Debug"
		kind "ConsoleApp"
		defines "PSTC_DEBUG"
		symbols "On"
		
	filter "configurations:VerboseDebug"
		kind "ConsoleApp"
		defines {"PSTC_DEBUG", "PSTC_VERBOSE_DEBUG"}
		symbols "On"

	filter "configurations:Release"
		kind "WindowedApp"
		linkoptions {"/ENTRY:\"mainCRTStartup\""}
		defines "PSTC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		kind "WindowedApp"
		linkoptions {"/ENTRY:\"mainCRTStartup\""}
		defines "PSTC_DIST"
		optimize "On"
