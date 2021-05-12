---
--- PepperMint Dependencies
---

VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/PepperMint/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/PepperMint/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/PepperMint/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/PepperMint/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/PepperMint/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/PepperMint/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/PepperMint/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/PepperMint/vendor/ImGuizmo"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

-- Library directories relative to root folder (solution directory)
LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/PepperMint/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_Debug_DLL"] = "%{wks.location}/PepperMint/vendor/VulkanSDK/Bin"

-- Library declarations (.lib)
Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
