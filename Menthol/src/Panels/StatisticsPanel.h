#pragma once

#include <string>

namespace PepperMint {

class StatisticsPanel {
  public:
    StatisticsPanel()  = default;
    ~StatisticsPanel() = default;

    void onImGuiRender();

    std::string currentFile() const { return _currentFile; }
    void        setCurrentFile(const std::string& iFile) { _currentFile = iFile; }

  private:
    std::string _currentFile;
};
}
