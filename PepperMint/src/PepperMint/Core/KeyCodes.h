#pragma once

namespace PepperMint {

using Key = enum class KeyCode : int {
	// From glfw3.h
	SPACE			= 32,
	APOSTROPHE      = 39, /* ' */
	COMMA           = 44, /* , */
	MINUS           = 45, /* - */
	PERIOD          = 46, /* . */
	SLASH           = 47, /* / */

	D0              = 48, /* 0 */
	D1              = 49, /* 1 */
	D2              = 50, /* 2 */
	D3              = 51, /* 3 */
	D4              = 52, /* 4 */
	D5              = 53, /* 5 */
	D6              = 54, /* 6 */
	D7              = 55, /* 7 */
	D8              = 56, /* 8 */
	D9              = 57, /* 9 */

	SEMICOLON       = 59, /* ; */
	EQUAL           = 61, /* = */

	A               = 65,
	B               = 66,
	C               = 67,
	D               = 68,
	E               = 69,
	F               = 70,
	G               = 71,
	H               = 72,
	I               = 73,
	J               = 74,
	K               = 75,
	L               = 76,
	M               = 77,
	N               = 78,
	O               = 79,
	P               = 80,
	Q               = 81,
	R               = 82,
	S               = 83,
	T               = 84,
	U               = 85,
	V               = 86,
	W               = 87,
	X               = 88,
	Y               = 89,
	Z               = 90,

	LEFT_BRACKET    = 91, /* [ */
	BACKSLASH       = 92, /* \ */
	RIGHT_BRACKET   = 93, /* ] */
	GRAVE_ACCENT    = 96, /* ` */

	WORLD_1         = 161, /* non-US #1 */
	WORLD_2         = 162, /* non-US #2 */

	// Function keys
	ESCAPE          = 256,
	ENTER           = 257,
	TAB             = 258,
	BACKSPACE       = 259,
	INSERT          = 260,
	DEL				= 261,
	RIGHT           = 262,
	LEFT            = 263,
	DOWN            = 264,
	UP              = 265,
	PAGE_UP         = 266,
	PAGE_DOWN       = 267,
	HOME            = 268,
	END             = 269,
	CAPS_LOCK       = 280,
	SCROLL_LOCK     = 281,
	NUM_LOCK        = 282,
	PRINT_SCREEN    = 283,
	PAUSE           = 284,
	F1              = 290,
	F2              = 291,
	F3              = 292,
	F4              = 293,
	F5              = 294,
	F6              = 295,
	F7              = 296,
	F8              = 297,
	F9              = 298,
	F10             = 299,
	F11             = 300,
	F12             = 301,
	F13             = 302,
	F14             = 303,
	F15             = 304,
	F16             = 305,
	F17             = 306,
	F18             = 307,
	F19             = 308,
	F20             = 309,
	F21             = 310,
	F22             = 311,
	F23             = 312,
	F24             = 313,
	F25             = 314,

	// Keypad
	KP0             = 320,
	KP1             = 321,
	KP2             = 322,
	KP3             = 323,
	KP4             = 324,
	KP5             = 325,
	KP6             = 326,
	KP7             = 327,
	KP8             = 328,
	KP9             = 329,
	KP_DECIMAL      = 330,
	KP_DIVIDE       = 331,
	KP_MULTIPLY     = 332,
	KP_SUBTRACT     = 333,
	KP_ADD          = 334,
	KP_ENTER        = 335,
	KP_EQUAL        = 336,

	LEFT_SHIFT      = 340,
	LEFT_CONTROL    = 341,
	LEFT_ALT        = 342,
	LEFT_SUPER      = 343,
	RIGHT_SHIFT     = 344,
	RIGHT_CONTROL	= 345,
	RIGHT_ALT       = 346,
	RIGHT_SUPER     = 347,
	MENU            = 348,
};

inline std::ostream& operator<<(std::ostream& os, KeyCode iKeyCode) {
	os << static_cast<int>(iKeyCode);
	return os;
}
}

#define PM_KEY_SPACE              ::PepperMint::Key::SPACE
#define PM_KEY_APOSTROPHE         ::PepperMint::Key::APOSTROPHE
#define PM_KEY_COMMA              ::PepperMint::Key::COMMA
#define PM_KEY_MINUS              ::PepperMint::Key::MINUS
#define PM_KEY_PERIOD             ::PepperMint::Key::PERIOD
#define PM_KEY_SLASH              ::PepperMint::Key::SLASH

#define PM_KEY_0                  ::PepperMint::Key::D0
#define PM_KEY_1                  ::PepperMint::Key::D1
#define PM_KEY_2                  ::PepperMint::Key::D2
#define PM_KEY_3                  ::PepperMint::Key::D3
#define PM_KEY_4                  ::PepperMint::Key::D4
#define PM_KEY_5                  ::PepperMint::Key::D5
#define PM_KEY_6                  ::PepperMint::Key::D6
#define PM_KEY_7                  ::PepperMint::Key::D7
#define PM_KEY_8                  ::PepperMint::Key::D8
#define PM_KEY_9                  ::PepperMint::Key::D9

#define PM_KEY_SEMICOLON          ::PepperMint::Key::SEMICOLON
#define PM_KEY_EQUAL              ::PepperMint::Key::EQUAL

#define PM_KEY_A                  ::PepperMint::Key::A
#define PM_KEY_B                  ::PepperMint::Key::B
#define PM_KEY_C                  ::PepperMint::Key::C
#define PM_KEY_D                  ::PepperMint::Key::D
#define PM_KEY_E                  ::PepperMint::Key::E
#define PM_KEY_F                  ::PepperMint::Key::F
#define PM_KEY_G                  ::PepperMint::Key::G
#define PM_KEY_H                  ::PepperMint::Key::H
#define PM_KEY_I                  ::PepperMint::Key::I
#define PM_KEY_J                  ::PepperMint::Key::J
#define PM_KEY_K                  ::PepperMint::Key::K
#define PM_KEY_L                  ::PepperMint::Key::L
#define PM_KEY_M                  ::PepperMint::Key::M
#define PM_KEY_N                  ::PepperMint::Key::N
#define PM_KEY_O                  ::PepperMint::Key::O
#define PM_KEY_P                  ::PepperMint::Key::P
#define PM_KEY_Q                  ::PepperMint::Key::Q
#define PM_KEY_R                  ::PepperMint::Key::R
#define PM_KEY_S                  ::PepperMint::Key::S
#define PM_KEY_T                  ::PepperMint::Key::T
#define PM_KEY_U                  ::PepperMint::Key::U
#define PM_KEY_V                  ::PepperMint::Key::V
#define PM_KEY_W                  ::PepperMint::Key::W
#define PM_KEY_X                  ::PepperMint::Key::X
#define PM_KEY_Y                  ::PepperMint::Key::Y
#define PM_KEY_Z                  ::PepperMint::Key::Z

#define PM_KEY_LEFT_BRACKET       ::PepperMint::Key::LEFT_BRACKET
#define PM_KEY_BACKSLASH          ::PepperMint::Key::BACKSLASH
#define PM_KEY_RIGHT_BRACKET      ::PepperMint::Key::RIGHT_BRACKET
#define PM_KEY_GRAVE_ACCENT       ::PepperMint::Key::GRAVE_ACCENT
#define PM_KEY_WORLD_1            ::PepperMint::Key::WORLD_1
#define PM_KEY_WORLD_2            ::PepperMint::Key::WORLD_2

#define PM_KEY_ESCAPE             ::PepperMint::Key::ESCAPE
#define PM_KEY_ENTER              ::PepperMint::Key::ENTER
#define PM_KEY_TAB                ::PepperMint::Key::TAB
#define PM_KEY_BACKSPACE          ::PepperMint::Key::BACKSPACE
#define PM_KEY_INSERT             ::PepperMint::Key::INSERT
#define PM_KEY_DELETE             ::PepperMint::Key::DEL
#define PM_KEY_RIGHT              ::PepperMint::Key::RIGHT
#define PM_KEY_LEFT               ::PepperMint::Key::LEFT
#define PM_KEY_DOWN               ::PepperMint::Key::DOWN
#define PM_KEY_UP                 ::PepperMint::Key::UP
#define PM_KEY_PAGE_UP            ::PepperMint::Key::PAGE_UP
#define PM_KEY_PAGE_DOWN          ::PepperMint::Key::PAGE_DOWN
#define PM_KEY_HOME               ::PepperMint::Key::HOME
#define PM_KEY_END                ::PepperMint::Key::END
#define PM_KEY_CAPS_LOCK          ::PepperMint::Key::CAPS_LOCK
#define PM_KEY_SCROLL_LOCK        ::PepperMint::Key::SCROLL_LOCK
#define PM_KEY_NUM_LOCK           ::PepperMint::Key::NUM_LOCK
#define PM_KEY_PRINT_SCREEN       ::PepperMint::Key::PRINT_SCREEN
#define PM_KEY_PAUSE              ::PepperMint::Key::PAUSE

#define PM_KEY_F1                 ::PepperMint::Key::F1
#define PM_KEY_F2                 ::PepperMint::Key::F2
#define PM_KEY_F3                 ::PepperMint::Key::F3
#define PM_KEY_F4                 ::PepperMint::Key::F4
#define PM_KEY_F5                 ::PepperMint::Key::F5
#define PM_KEY_F6                 ::PepperMint::Key::F6
#define PM_KEY_F7                 ::PepperMint::Key::F7
#define PM_KEY_F8                 ::PepperMint::Key::F8
#define PM_KEY_F9                 ::PepperMint::Key::F9
#define PM_KEY_F10                ::PepperMint::Key::F10
#define PM_KEY_F11                ::PepperMint::Key::F11
#define PM_KEY_F12                ::PepperMint::Key::F12
#define PM_KEY_F13                ::PepperMint::Key::F13
#define PM_KEY_F14                ::PepperMint::Key::F14
#define PM_KEY_F15                ::PepperMint::Key::F15
#define PM_KEY_F16                ::PepperMint::Key::F16
#define PM_KEY_F17                ::PepperMint::Key::F17
#define PM_KEY_F18                ::PepperMint::Key::F18
#define PM_KEY_F19                ::PepperMint::Key::F19
#define PM_KEY_F20                ::PepperMint::Key::F20
#define PM_KEY_F21                ::PepperMint::Key::F21
#define PM_KEY_F22                ::PepperMint::Key::F22
#define PM_KEY_F23                ::PepperMint::Key::F23
#define PM_KEY_F24                ::PepperMint::Key::F24
#define PM_KEY_F25                ::PepperMint::Key::F25

#define PM_KEY_KP_0               ::PepperMint::Key::KP0
#define PM_KEY_KP_1               ::PepperMint::Key::KP1
#define PM_KEY_KP_2               ::PepperMint::Key::KP2
#define PM_KEY_KP_3               ::PepperMint::Key::KP3
#define PM_KEY_KP_4               ::PepperMint::Key::KP4
#define PM_KEY_KP_5               ::PepperMint::Key::KP5
#define PM_KEY_KP_6               ::PepperMint::Key::KP6
#define PM_KEY_KP_7               ::PepperMint::Key::KP7
#define PM_KEY_KP_8               ::PepperMint::Key::KP8
#define PM_KEY_KP_9               ::PepperMint::Key::KP9
#define PM_KEY_KP_DECIMAL         ::PepperMint::Key::KP_DECIMAL
#define PM_KEY_KP_DIVIDE          ::PepperMint::Key::KP_DIVIDE
#define PM_KEY_KP_MULTIPLY        ::PepperMint::Key::KP_MULTIPLY
#define PM_KEY_KP_SUBTRACT        ::PepperMint::Key::KP_SUBTRACT
#define PM_KEY_KP_ADD             ::PepperMint::Key::KP_ADD
#define PM_KEY_KP_ENTER           ::PepperMint::Key::KP_ENTER
#define PM_KEY_KP_EQUAL           ::PepperMint::Key::KP_EQUAL

#define PM_KEY_LEFT_SHIFT         ::PepperMint::Key::LEFT_SHIFT
#define PM_KEY_LEFT_CONTROL       ::PepperMint::Key::LEFT_CONTROL
#define PM_KEY_LEFT_ALT           ::PepperMint::Key::LEFT_ALT
#define PM_KEY_LEFT_SUPER         ::PepperMint::Key::LEFT_SUPER
#define PM_KEY_RIGHT_SHIFT        ::PepperMint::Key::RIGHT_SHIFT
#define PM_KEY_RIGHT_CONTROL      ::PepperMint::Key::RIGHT_CONTROL
#define PM_KEY_RIGHT_ALT          ::PepperMint::Key::RIGHT_ALT
#define PM_KEY_RIGHT_SUPER        ::PepperMint::Key::RIGHT_SUPER
#define PM_KEY_MENU               ::PepperMint::Key::MENU