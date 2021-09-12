# PepperMint [![License](https://img.shields.io/github/license/Naheuldark/PepperMint.svg)](https://github.com/Naheuldark/PepperMint/blob/master/LICENSE)

![PepperMint](/Resources/Branding/PepperMintLogo.png?raw=true "PepperMint") ![Menthol](/Resources/Branding/MentholLogo.png?raw=true "Menthol")

## Getting started
PepperMint is primarily an early-stage interactive application and rendering engine for Windows.<br>
It is based on the [_TheCherno_](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)'s project called [Hazel](https://github.com/TheCherno/Hazel).

<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/Naheuldark/PepperMint`.
If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

<ins>**2. Configuring the dependencies:**</ins>

1. Run the [Win-GenerateProject.bat](https://github.com/Naheuldark/PepperMint/blob/master/scripts/Win-GenerateProject.bat) file found in `scripts` folder. This will download the required prerequisites for the project if they are not present yet.
2. One prerequisite is the Vulkan SDK. If it is not installed, the script will execute the `VulkanSDK.exe` file, and will prompt the user to install the SDK.
3. After installation, run the [Win-GenerateProject.bat](https://github.com/Naheuldark/PepperMint/blob/master/scripts/Win-GenerateProject.bat) file again. If the Vulkan SDK is installed properly, it will then download the Vulkan SDK Debug libraries. (This may take a longer amount of time)
4. After donwloading and unzipping the files, the setup will continue automatically, which will then generate a Visual Studio solution file for user's usage.

If changes are made, or if you want to regenerate project files, rerun the [Win-GenerateProject.bat](https://github.com/Naheuldark/PepperMint/blob/master/scripts/Win-GenerateProject.bat) script file found in `scripts` folder.

### Main features to come:
- Fast 2D rendering (UI, particles, sprites, etc.)
- Support for Mac, Linux, Android and iOS
    - Native rendering API support (DirectX, Vulkan, Metal)
- Fully featured viewer and editor applications
- Fully scripted interaction and behavior
- Integrated 3rd party 2D and 3D physics engine
- Procedural terrain and world generation
- Artificial Intelligence
- Audio system

## Menthol Editor
As of 12SEP2021, the editor state is as shown below:
![PepperMint](/Resources/Captures/MentholEditor.png?raw=true "MentholEditor")

## Documentation
For more information, refer to the [official PepperMint documentation](https://naheuldark.github.io/PepperMint/).
