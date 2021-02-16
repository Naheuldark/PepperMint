#pragma once

#include <PepperMint.h>

namespace PepperMint {

class StatisticsPanel {
  public:
    StatisticsPanel()  = default;
    ~StatisticsPanel() = default;

    void onImGuiRender();

    std::string currentFile() const { return _currentFile; }
    void        setCurrentFile(const std::string& iFile) { _currentFile = iFile; }

    void setHoveredEntity(const Entity& iHoveredEntity) { _hoveredEntity = iHoveredEntity; }

  private:
    std::string _currentFile;
    Entity      _hoveredEntity{};
};
}
