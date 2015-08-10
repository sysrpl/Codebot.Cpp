#pragma once
#include <Codebot/NotifyEvent.h>
#include <Codebot/Input/KeyModifiers.h>
#include <Codebot/Input/KeyCodes.h>

namespace Codebot
{
namespace Input
{

// MouseEventArgs class

enum class MouseButton
{
	Left = 1,
	Middle = Left << 1,
	Right = Middle << 1,
	WheelUp = Right << 1,
	WheelDown = WheelUp << 1
};

typedef Flags<MouseButton> MouseButtons;

class MouseEventArgs : public EventArgs
{
public:
	KeyModifiers Modifiers;
	MouseButtons Buttons;
	Integer X, Y;
	Integer DeltaX, DeltaY;
};

// MouseEvent class

class MouseEvent : public EventHandler<Object, MouseEventArgs>
{
};

MouseEvent::Method Event(const MouseEvent::Handler& code);

}
}
