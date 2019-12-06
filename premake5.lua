workspace "PepperMint"
	architecture "x64"

	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "PepperMint/vendor/GLFW/include"
IncludeDir["Glad"] = "PepperMint/vendor/Glad/include"
IncludeDir["ImGui"] = "PepperMint/vendor/imgui"

-- Include external premake files
include "PepperMint/vendor/GLFW"
include "PepperMint/vendor/Glad"
include "PepperMint/vendor/imgui"

project "PepperMint"
	location "PepperMint"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pmpch.h"
	pchsource "PepperMint/src/pmpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Glad}"
	}

	links {
		"GLFW",
		"ImGui",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
			"PM_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PM_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"PepperMint/vendor/spdlog/include",
		"PepperMint/src"
	}

	links {
		"PepperMint"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "PM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PM_DIST"
		runtime "Release"
		optimize "On"