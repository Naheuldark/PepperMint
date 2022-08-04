---
--- PepperMint Dependencies
---

VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/PepperMint/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/PepperMint/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/PepperMint/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/PepperMint/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/PepperMint/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/PepperMint/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/PepperMint/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/PepperMint/vendor/ImGuizmo"
IncludeDir["Box2D"] = "%{wks.location}/PepperMint/vendor/box2d/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["mono"] = "%{wks.location}/PepperMint/vendor/mono/include"

-- Library directories relative to root folder (solution directory)
LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["mono"] = "%{wks.location}/PepperMint/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["monoDLL"] = "%{wks.location}/PepperMint/vendor/mono/dll/%{cfg.buildcfg}"

-- Library declarations (.lib)
Library = {}
Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
