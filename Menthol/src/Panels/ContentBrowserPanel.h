#pragma once

#include <filesystem>

#include <PepperMint/Renderer/Texture.h>

namespace PepperMint {

class ContentBrowserPanel {
  public:
    ContentBrowserPanel();
    ~ContentBrowserPanel() = default;

    void onImGuiRender();

  private:
    std::filesystem::path _currentDirectory;

	// Icons
	Ref<Texture2D> _directoryIcon;
    Ref<Texture2D> _fileIcon;
};
}
