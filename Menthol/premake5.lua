---
--- Menthol
---

project "Menthol"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")	
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{wks.location}/PepperMint/vendor/spdlog/include",
		"%{wks.location}/PepperMint/src",
		"%{wks.location}/PepperMint/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links {
		"PepperMint"
	}

	postbuildcommands {
		"{COPYDIR} \"%{LibraryDir.monoDLL}\" \"%{cfg.targetdir}\""
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PM_DIST"
		runtime "Release"
		optimize "on"
