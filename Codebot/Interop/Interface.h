#pragma once
#include <Codebot/ValueType.h>
#include <Codebot/String.h>
#include <Codebot/Console.h>

namespace Codebot
{
namespace Interop
{

extern const Result ROk;
extern const Result RFalse;
extern const Result RNoInterface;
extern const Result RUnexpected;
extern const Result RNotImplplemented;

// Interface

template <typename T>
class Interface : public ValueType
{
private:
	T* instance;
	void Lock() const;
	void Unlock();
public:
	Interface();
	Interface(T* value);
	Interface(const Interface<T>& value);
	template <typename I>
	Interface(const Interface<I>& value);
	~Interface();
	Interface<T>& operator = (const Interface<T>& value);
	operator T* () const;
	T* operator -> () const;
	Boolean HasValue() const;
	T* In();
	T*& Out();
};

// Interface querying

template <typename T, typename I>
Interface<T> QueryAs(const Interface<I>& value)
{
	if (!value.HasValue())
		return Interface<T>();
	Pointer p;
	if (value->QueryInterface(T::Id, p) == ROk)
		return Interface<T>((T*)p);
	else
		return Interface<T>();
}

template <typename T, typename I>
Boolean QueryIs(const Interface<I>& value)
{
	return QueryAs<T>(value).HasValue();
}

// Interface

template <typename T>
Interface<T>::Interface()
	: instance(null)
{
}

template <typename T>
Interface<T>::Interface(T* value)
	: instance(value)
{
}

template <typename T>
Interface<T>::Interface(const Interface<T>& value)
	: instance(value.instance)
{
	Lock();
}

template <typename T>
template <typename I>
Interface<T>::Interface(const Interface<I>& value)
	: instance(QueryAs<T>(value).instance)
{
	Lock();
}

template <typename T>
Interface<T>::~Interface()
{
	Unlock();
}

template <typename T>
void Interface<T>::Lock() const
{
	if (instance != null)
		instance->AddRef();
}

template <typename T>
void Interface<T>::Unlock()
{
	if (instance != null)
		instance->Release();
	instance = null;
}

template <typename T>
Interface<T>& Interface<T>::operator = (const Interface<T>& value)
{
	if (instance != value.instance)
	{
		Unlock();
		instance = value.instance;
		Lock();
	}
	return *this;
}

template <typename T>
Interface<T>::operator T* () const
{
	Lock();
	return instance;	
}

template <typename T>
T* Interface<T>::operator -> () const
{
	if (instance == null)
		ThrowNullReferenceException(ThisMethod, "instance");
	return instance;
}

template <typename T>
T* Interface<T>::In()
{
	Lock();
	return instance;
}

template <typename T>
T*& Interface<T>::Out()
{
	return instance;
}

template <typename T>
bool Interface<T>::HasValue() const
{
	return instance != null;
}

// Interface parameter shortcuts

template <typename T>
inline T* In(Interface<T>& value)
{
	return value.In();
}

template <typename T>
inline T*& Out(Interface<T>& value)
{
	return value.Out();
}

}
}
