#include <Codebot/Input/KeyboardEvent.h>

namespace Codebot
{
namespace Input
{

KeyboardEvent::Method Event(const KeyboardEvent::Handler& code)
{
	return KeyboardEvent::Method(code);
}

}
}
