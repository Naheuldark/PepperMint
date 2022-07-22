#pragma once

#include <filesystem>

#include <PepperMint.h>

namespace Menthol {

class ContentBrowserPanel {
  public:
    ContentBrowserPanel();
    ~ContentBrowserPanel() = default;

    void onImGuiRender();

  private:
    std::filesystem::path _currentDirectory;

    // Icons
    PepperMint::Ref<PepperMint::Texture2D> _directoryIcon, _fileIcon;
};
}
