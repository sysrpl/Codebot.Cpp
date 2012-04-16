#include "Codebot/NotifyEvent.h"

namespace Codebot
{

EventArgs EventArgs::Empty = EventArgs();

EventArgs::operator EventArgs* ()
{
	return this;
}


NotifyEvent::Method Event(Object* data, const NotifyEvent::Handler& code)
{
	return NotifyEvent::Method(data, code);
}

}