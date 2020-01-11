#pragma once

namespace PepperMint {

using Mouse = enum class MouseCode : int {
	// From glfw3.h
	BUTTON_1        = 0,
	BUTTON_2        = 1,
	BUTTON_3        = 2,
	BUTTON_4        = 3,
	BUTTON_5        = 4,
	BUTTON_6        = 5,
	BUTTON_7        = 6,
	BUTTON_8        = 7,
	BUTTON_LAST     = BUTTON_8,
	BUTTON_LEFT     = BUTTON_1,
	BUTTON_RIGHT    = BUTTON_2,
	BUTTON_MIDDLE	= BUTTON_3,
};

inline std::ostream& operator<<(std::ostream& os, MouseCode iMouseCode) {
	os << static_cast<int>(iMouseCode);
	return os;
}
}

#define PM_MOUSE_BUTTON_1         ::PepperMint::Mouse::BUTTON_1
#define PM_MOUSE_BUTTON_2         ::PepperMint::Mouse::BUTTON_2
#define PM_MOUSE_BUTTON_3         ::PepperMint::Mouse::BUTTON_3
#define PM_MOUSE_BUTTON_4         ::PepperMint::Mouse::BUTTON_4
#define PM_MOUSE_BUTTON_5         ::PepperMint::Mouse::BUTTON_5
#define PM_MOUSE_BUTTON_6         ::PepperMint::Mouse::BUTTON_6
#define PM_MOUSE_BUTTON_7         ::PepperMint::Mouse::BUTTON_7
#define PM_MOUSE_BUTTON_8         ::PepperMint::Mouse::BUTTON_8
#define PM_MOUSE_BUTTON_LAST      ::PepperMint::Mouse::BUTTON_LAST
#define PM_MOUSE_BUTTON_LEFT      ::PepperMint::Mouse::BUTTON_LEFT
#define PM_MOUSE_BUTTON_RIGHT     ::PepperMint::Mouse::BUTTON_RIGHT
#define PM_MOUSE_BUTTON_MIDDLE	  ::PepperMint::Mouse::BUTTON_MIDDLE