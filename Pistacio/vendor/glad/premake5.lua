project "glad"
	location "glad"
	kind "StaticLib"
	language "C"

	targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}/target")
	objdir ("../../../bin/" .. outputdir .. "/%{prj.name}/obj")

	files
	{
		"glad/include/glad/glad.h",
		"glad/include/KHR/khrplatform.h",
		"glad/src/glad.c"
	}
	
	includedirs
	{
		"glad/include"
	}
	
	filter "system:windows"		
		systemversion "latest"
		staticruntime "On"