#pragma once

#include <filesystem>

#include <PepperMint.h>

namespace PepperMint {

class ContentBrowserPanel {
  public:
    ContentBrowserPanel();
    ~ContentBrowserPanel() = default;

    void onImGuiRender();

  private:
    std::filesystem::path _currentDirectory;

    // Icons
    Ref<Texture2D> _directoryIcon, _fileIcon;
};
}
