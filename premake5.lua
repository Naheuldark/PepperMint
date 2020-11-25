include "./vendor/premake/premake_custom/solution_items.lua"

workspace "PepperMint"
	architecture "x86_64"

	startproject "Menthol"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	solution_items {
		".editorconfig"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/PepperMint/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/PepperMint/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/PepperMint/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/PepperMint/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/PepperMint/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/PepperMint/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/PepperMint/vendor/yaml-cpp/include"

-- Include external premake files
group "Dependencies"
	include "vendor/premake"
	include "PepperMint/vendor/GLFW"
	include "PepperMint/vendor/Glad"
	include "PepperMint/vendor/imgui"
	include "PepperMint/vendor/yaml-cpp"
group ""

include "PepperMint"
include "Menthol"
include "Sandbox"
