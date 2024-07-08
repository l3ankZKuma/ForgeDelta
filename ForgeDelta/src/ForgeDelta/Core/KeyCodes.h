#pragma once

#include <ostream>

namespace ForgeDelta
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	enum class KeyState
	{
		None = -1,
		Pressed,
		Held,
		Released
	};

	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};

	typedef enum class MouseButton : uint16_t
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Left = Button0,
		Right = Button1,
		Middle = Button2
	} Button;


	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, MouseButton button)
	{
		os << static_cast<int32_t>(button);
		return os;
	}
}

// From glfw3.h
#define FD_KEY_SPACE           ::ForgeDelta::Key::Space
#define FD_KEY_APOSTROPHE      ::ForgeDelta::Key::Apostrophe    /* ' */
#define FD_KEY_COMMA           ::ForgeDelta::Key::Comma         /* , */
#define FD_KEY_MINUS           ::ForgeDelta::Key::Minus         /* - */
#define FD_KEY_PERIOD          ::ForgeDelta::Key::Period        /* . */
#define FD_KEY_SLASH           ::ForgeDelta::Key::Slash         /* / */
#define FD_KEY_0               ::ForgeDelta::Key::D0
#define FD_KEY_1               ::ForgeDelta::Key::D1
#define FD_KEY_2               ::ForgeDelta::Key::D2
#define FD_KEY_3               ::ForgeDelta::Key::D3
#define FD_KEY_4               ::ForgeDelta::Key::D4
#define FD_KEY_5               ::ForgeDelta::Key::D5
#define FD_KEY_6               ::ForgeDelta::Key::D6
#define FD_KEY_7               ::ForgeDelta::Key::D7
#define FD_KEY_8               ::ForgeDelta::Key::D8
#define FD_KEY_9               ::ForgeDelta::Key::D9
#define FD_KEY_SEMICOLON       ::ForgeDelta::Key::Semicolon     /* ; */
#define FD_KEY_EQUAL           ::ForgeDelta::Key::Equal         /* = */
#define FD_KEY_A               ::ForgeDelta::Key::A
#define FD_KEY_B               ::ForgeDelta::Key::B
#define FD_KEY_C               ::ForgeDelta::Key::C
#define FD_KEY_D               ::ForgeDelta::Key::D
#define FD_KEY_E               ::ForgeDelta::Key::E
#define FD_KEY_F               ::ForgeDelta::Key::F
#define FD_KEY_G               ::ForgeDelta::Key::G
#define FD_KEY_H               ::ForgeDelta::Key::H
#define FD_KEY_I               ::ForgeDelta::Key::I
#define FD_KEY_J               ::ForgeDelta::Key::J
#define FD_KEY_K               ::ForgeDelta::Key::K
#define FD_KEY_L               ::ForgeDelta::Key::L
#define FD_KEY_M               ::ForgeDelta::Key::M
#define FD_KEY_N               ::ForgeDelta::Key::N
#define FD_KEY_O               ::ForgeDelta::Key::O
#define FD_KEY_P               ::ForgeDelta::Key::P
#define FD_KEY_Q               ::ForgeDelta::Key::Q
#define FD_KEY_R               ::ForgeDelta::Key::R
#define FD_KEY_S               ::ForgeDelta::Key::S
#define FD_KEY_T               ::ForgeDelta::Key::T
#define FD_KEY_U               ::ForgeDelta::Key::U
#define FD_KEY_V               ::ForgeDelta::Key::V
#define FD_KEY_W               ::ForgeDelta::Key::W
#define FD_KEY_X               ::ForgeDelta::Key::X
#define FD_KEY_Y               ::ForgeDelta::Key::Y
#define FD_KEY_Z               ::ForgeDelta::Key::Z
#define FD_KEY_LEFT_BRACKET    ::ForgeDelta::Key::LeftBracket   /* [ */
#define FD_KEY_BACKSLASH       ::ForgeDelta::Key::Backslash     /* \ */
#define FD_KEY_RIGHT_BRACKET   ::ForgeDelta::Key::RightBracket  /* ] */
#define FD_KEY_GRAVE_ACCENT    ::ForgeDelta::Key::GraveAccent   /* ` */
#define FD_KEY_WORLD_1         ::ForgeDelta::Key::World1        /* non-US #1 */
#define FD_KEY_WORLD_2         ::ForgeDelta::Key::World2        /* non-US #2 */

/* Function keys */
#define FD_KEY_ESCAPE          ::ForgeDelta::Key::Escape
#define FD_KEY_ENTER           ::ForgeDelta::Key::Enter
#define FD_KEY_TAB             ::ForgeDelta::Key::Tab
#define FD_KEY_BACKSPACE       ::ForgeDelta::Key::Backspace
#define FD_KEY_INSERT          ::ForgeDelta::Key::Insert
#define FD_KEY_DELETE          ::ForgeDelta::Key::Delete
#define FD_KEY_RIGHT           ::ForgeDelta::Key::Right
#define FD_KEY_LEFT            ::ForgeDelta::Key::Left
#define FD_KEY_DOWN            ::ForgeDelta::Key::Down
#define FD_KEY_UP              ::ForgeDelta::Key::Up
#define FD_KEY_PAGE_UP         ::ForgeDelta::Key::PageUp
#define FD_KEY_PAGE_DOWN       ::ForgeDelta::Key::PageDown
#define FD_KEY_HOME            ::ForgeDelta::Key::Home
#define FD_KEY_END             ::ForgeDelta::Key::End
#define FD_KEY_CAPS_LOCK       ::ForgeDelta::Key::CapsLock
#define FD_KEY_SCROLL_LOCK     ::ForgeDelta::Key::ScrollLock
#define FD_KEY_NUM_LOCK        ::ForgeDelta::Key::NumLock
#define FD_KEY_PRINT_SCREEN    ::ForgeDelta::Key::PrintScreen
#define FD_KEY_PAUSE           ::ForgeDelta::Key::Pause
#define FD_KEY_F1              ::ForgeDelta::Key::F1
#define FD_KEY_F2              ::ForgeDelta::Key::F2
#define FD_KEY_F3              ::ForgeDelta::Key::F3
#define FD_KEY_F4              ::ForgeDelta::Key::F4
#define FD_KEY_F5              ::ForgeDelta::Key::F5
#define FD_KEY_F6              ::ForgeDelta::Key::F6
#define FD_KEY_F7              ::ForgeDelta::Key::F7
#define FD_KEY_F8              ::ForgeDelta::Key::F8
#define FD_KEY_F9              ::ForgeDelta::Key::F9
#define FD_KEY_F10             ::ForgeDelta::Key::F10
#define FD_KEY_F11             ::ForgeDelta::Key::F11
#define FD_KEY_F12             ::ForgeDelta::Key::F12
#define FD_KEY_F13             ::ForgeDelta::Key::F13
#define FD_KEY_F14             ::ForgeDelta::Key::F14
#define FD_KEY_F15             ::ForgeDelta::Key::F15
#define FD_KEY_F16             ::ForgeDelta::Key::F16
#define FD_KEY_F17             ::ForgeDelta::Key::F17
#define FD_KEY_F18             ::ForgeDelta::Key::F18
#define FD_KEY_F19             ::ForgeDelta::Key::F19
#define FD_KEY_F20             ::ForgeDelta::Key::F20
#define FD_KEY_F21             ::ForgeDelta::Key::F21
#define FD_KEY_F22             ::ForgeDelta::Key::F22
#define FD_KEY_F23             ::ForgeDelta::Key::F23
#define FD_KEY_F24             ::ForgeDelta::Key::F24
#define FD_KEY_F25             ::ForgeDelta::Key::F25

/* Keypad */
#define FD_KEY_KP_0            ::ForgeDelta::Key::KP0
#define FD_KEY_KP_1            ::ForgeDelta::Key::KP1
#define FD_KEY_KP_2            ::ForgeDelta::Key::KP2
#define FD_KEY_KP_3            ::ForgeDelta::Key::KP3
#define FD_KEY_KP_4            ::ForgeDelta::Key::KP4
#define FD_KEY_KP_5            ::ForgeDelta::Key::KP5
#define FD_KEY_KP_6            ::ForgeDelta::Key::KP6
#define FD_KEY_KP_7            ::ForgeDelta::Key::KP7
#define FD_KEY_KP_8            ::ForgeDelta::Key::KP8
#define FD_KEY_KP_9            ::ForgeDelta::Key::KP9
#define FD_KEY_KP_DECIMAL      ::ForgeDelta::Key::KPDecimal
#define FD_KEY_KP_DIVIDE       ::ForgeDelta::Key::KPDivide
#define FD_KEY_KP_MULTIPLY     ::ForgeDelta::Key::KPMultiply
#define FD_KEY_KP_SUBTRACT     ::ForgeDelta::Key::KPSubtract
#define FD_KEY_KP_ADD          ::ForgeDelta::Key::KPAdd
#define FD_KEY_KP_ENTER        ::ForgeDelta::Key::KPEnter
#define FD_KEY_KP_EQUAL        ::ForgeDelta::Key::KPEqual

#define FD_KEY_LEFT_SHIFT      ::ForgeDelta::Key::LeftShift
#define FD_KEY_LEFT_CONTROL    ::ForgeDelta::Key::LeftControl
#define FD_KEY_LEFT_ALT        ::ForgeDelta::Key::LeftAlt
#define FD_KEY_LEFT_SUPER      ::ForgeDelta::Key::LeftSuper
#define FD_KEY_RIGHT_SHIFT     ::ForgeDelta::Key::RightShift
#define FD_KEY_RIGHT_CONTROL   ::ForgeDelta::Key::RightControl
#define FD_KEY_RIGHT_ALT       ::ForgeDelta::Key::RightAlt
#define FD_KEY_RIGHT_SUPER     ::ForgeDelta::Key::RightSuper
#define FD_KEY_MENU            ::ForgeDelta::Key::Menu

// Mouse
#define FD_MOUSE_BUTTON_LEFT    ::ForgeDelta::Button::Left
#define FD_MOUSE_BUTTON_RIGHT   ::ForgeDelta::Button::Right
#define FD_MOUSE_BUTTON_MIDDLE  ::ForgeDelta::Button::Middle
