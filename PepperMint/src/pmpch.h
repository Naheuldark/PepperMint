#pragma once

#include "PepperMint/Core/PlatformDetection.h"

#ifdef PM_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif // !NOMINMAX
#endif // PM_PLATFORM_WINDOWS

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <sstream>
#include <string>

#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PepperMint/Core/Base.h"
#include "PepperMint/Core/Log.h"
#include "PepperMint/Debug/Instrumentor.h"

#ifdef PM_PLATFORM_WINDOWS
#include <Windows.h>
#endif // PM_PLATFORM_WINDOWS
