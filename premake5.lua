---
--- PepperMint Workspace
---

include "./vendor/premake/premake_custom/solution_items.lua"
include "./dependencies.lua"

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

-- Include external premake files
group "Dependencies"
	include "vendor/premake"
	include "PepperMint/vendor/GLFW"
	include "PepperMint/vendor/box2d"
	include "PepperMint/vendor/Glad"
	include "PepperMint/vendor/imgui"
	include "PepperMint/vendor/yaml-cpp"
group ""

group "Core"
	include "PepperMint"
	include "PepperMint-ScriptCore"
group ""

group "Tools"
	include "Menthol"
group ""
