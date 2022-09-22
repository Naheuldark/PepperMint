---
--- PepperMint-ScriptCore
---

project "PepperMint-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"

	targetdir ("../Menthol/resources/scripts")	
	objdir ("../Menthol/resources/scripts/bin")

	files {
		"src/**.cs"
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
