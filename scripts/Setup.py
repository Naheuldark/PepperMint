import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
from SetupVulkan import VulkanConfiguration as VulkanRequirements

# Change from devtools/scripts directory to root
os.chdir('./../')

VulkanRequirements.Validate()

if (PremakeRequirements.Validate()):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        subprocess.call([os.path.abspath("vendor/premake/bin/premake5.exe"), "vs2019"])

    print("\nSetup completed!")
else:
    print("PepperMint requires Premake to generate project files.")
