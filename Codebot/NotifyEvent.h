#pragma once
#include <Codebot/Object.h>
#include <Codebot/EventHandler.h>

namespace Codebot
{

class EventArgs : public ValueType
{
public:
	static EventArgs Empty;
	operator EventArgs* ();
};

class NotifyEvent : public EventHandler<Object, EventArgs>
{
};

NotifyEvent::Method Event(Object* data, const NotifyEvent::Handler& code);

}