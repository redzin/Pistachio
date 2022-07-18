project "tinygltf"
	language "C++"
	cppdialect "C++11"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/target")
	objdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/obj")

	files {
		"tinygltf/loader_example.cc",
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		kind "ConsoleApp"
		defines {"PSTC_DEBUG", "DEBUG"}
		symbols "On"

	filter "configurations:VerboseDebug"
		kind "ConsoleApp"
		defines {"PSTC_DEBUG", "PSTC_VERBOSE_DEBUG"}
		symbols "On"

	filter "configurations:Release"
		kind "WindowedApp"
		linkoptions {"/ENTRY:\"mainCRTStartup\""}
		defines {"PSTC_RELEASE", "NDEBUG"}
		optimize "On"

	filter "configurations:Dist"
		kind "WindowedApp"
		linkoptions {"/ENTRY:\"mainCRTStartup\""}
		defines "PSTC_DIST"
		optimize "On"
