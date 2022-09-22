---
--- Sandbox Workspace
---

include "../vendor/premake/premake_custom/solution_items.lua"

workspace "Sandbox"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"

	targetdir ("bin")	
	objdir ("bin-int")

	files {
		"src/**.cs",
	}

	links {
		"PepperMint-ScriptCore"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "PepperMint-ScriptCore"
	include "../PepperMint-ScriptCore"
group ""
