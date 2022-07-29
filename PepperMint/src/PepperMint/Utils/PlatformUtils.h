#pragma once

#include <string>

namespace PepperMint {

class FileDialogs {
  public:
    // Return empty string if cancelled
    static std::string OpenFile(const char* iFilter);
    static std::string SaveFile(const char* iFilter);
};

class Time {
  public:
    static float GetTime();
};
}
