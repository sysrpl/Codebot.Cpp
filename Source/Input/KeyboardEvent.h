#pragma once
#include <Codebot/NotifyEvent.h>
#include <Codebot/Input/KeyModifiers.h>
#include <Codebot/Input/KeyCodes.h>

namespace Codebot
{
namespace Input
{

// KeyboardEventArgs class

class KeyboardEventArgs : public EventArgs
{
public:
	KeyModifiers Modifiers;
	KeyCode Code;
};

// KeyboardEvent class

class KeyboardEvent : public EventHandler<Object, KeyboardEventArgs>
{
};

KeyboardEvent::Method Event(const KeyboardEvent::Handler& code);

}
}
