#pragma once
#include <Codebot/NotifyEvent.h>
#include <Codebot/Flags.h>

namespace Codebot
{
namespace Input
{

enum class KeyModifier
{
	Control = 1,
	Shift = Control << 1,
	Alt = Shift << 1
};

typedef Flags<KeyModifier> KeyModifiers;

}
}
