#pragma once
#include "Codebot/Object.h"

namespace Codebot
{

template <typename Sender, typename Args>
class EventHandler : public ValueType
{
public:
	typedef void (*Handler)(Object*, Sender*, Args*);

	class Method : public ValueType
	{
	public:
		Object* Data;
		Handler Code;
		Method() { }
		Method(Object* d, const Handler& c); 
		Boolean operator == (const Method& m);
	};
private:
	Array<Method> methods;
public:
	void operator () (Sender* sender, Args* args);
	EventHandler<Sender, Args>& operator += (const Method& m);
	EventHandler<Sender, Args>& operator -= (const Method& m);
};

/* EventHandler<Sender, Args>::Method */

template <typename Sender, typename Args>
EventHandler<Sender, Args>::Method::Method(Object* d, const Handler& c) 
	: Data(d), Code(c) 
{ 
}

template <typename Sender, typename Args>
Boolean EventHandler<Sender, Args>::Method::operator == (const Method& m) 
{ 
	return m.Data == Data && m.Code == Code; 
}

/* EventHandler<Sender, Args> */

template <typename Sender, typename Args>
void EventHandler<Sender, Args>::operator () (Sender* sender, Args* args)
{
	Integer length = methods.Length();
	for (Integer i = 0; i < length; i++)
	{
		Method m = methods[i];
		m.Code(m.Data, sender, args);
	}
}

template <typename Sender, typename Args>
EventHandler<Sender, Args>& EventHandler<Sender, Args>::operator += (const Method& m)
{
	methods.Add(m);
	return *this;
}

template <typename Sender, typename Args>
EventHandler<Sender, Args>& EventHandler<Sender, Args>::operator -= (const Method& m)
{
	methods.Remove(m);
	return *this;
}

}