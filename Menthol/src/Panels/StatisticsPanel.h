#pragma once

#include <PepperMint.h>

namespace Menthol {

class StatisticsPanel {
  public:
    StatisticsPanel()  = default;
    ~StatisticsPanel() = default;

    void onImGuiRender();

    std::string currentFile() const { return _currentFile; }
    void        setCurrentFile(const std::string& iFile) { _currentFile = iFile; }

    void setHoveredEntity(const PepperMint::Entity& iHoveredEntity) { _hoveredEntity = iHoveredEntity; }

  private:
    std::string        _currentFile;
    PepperMint::Entity _hoveredEntity{};
};
}
