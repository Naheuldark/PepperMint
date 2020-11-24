#pragma once

namespace PepperMint {

class StatisticsPanel {
  public:
    StatisticsPanel() = default;
    ~StatisticsPanel() = default;

    void onImGuiRender();
};
}
