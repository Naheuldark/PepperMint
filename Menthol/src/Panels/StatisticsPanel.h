#pragma once

#include <string>

namespace PepperMint {

class StatisticsPanel {
  public:
    StatisticsPanel() = default;
    ~StatisticsPanel() = default;

    void onImGuiRender(const std::string& iCurrentFile);
};
}
