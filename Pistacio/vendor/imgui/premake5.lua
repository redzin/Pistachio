project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}/target")
	objdir ("../../../bin/" .. outputdir .. "/%{prj.name}/obj")

	files
	{
		"ImGui/imgui*.h",
		"ImGui/imgui*.cpp",
		"ImGui/backends/imgui_impl_glfw.h",
		"ImGui/backends/imgui_impl_glfw.cpp",
		"ImGui/backends/imgui_impl_opengl3.h",
		"ImGui/backends/imgui_impl_opengl3.cpp"
	}
	
	includedirs
	{
		"../GLFW/GLFW/include",
		"ImGui"
	}
	
	links
	{
		"GLFW"
	}
	
	filter "system:windows"		
		systemversion "latest"
		staticruntime "On"
