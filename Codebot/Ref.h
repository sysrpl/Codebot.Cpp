#pragma once
#include "Codebot/IManagedType.h"
#include "Codebot/String.h"

namespace Codebot
{

// Ref<T>

template <typename T>
class Ref : public ValueType
{
private:
	T* instance;
	void Lock();
	void Unlock();
public:
	Ref();
	Ref(T* object);
	Ref(const Ref<T>& ref);
	~Ref();
	operator T* () const;	
	T* operator -> () const;
	Ref<T>& operator = (const Ref<T>& ref);
	Ref<T>& operator = (T* object);
	Boolean operator == (T* object) const;
	void Clear();
	Boolean HasValue() const;
	T& Value() const;
	String ToFormat(const String& format) const;
};

// As<T> and Is<T>

template <typename T, typename R>
T* As(const Ref<R>& ref)
{
	return As<T>(ref.Object());
}

template <typename T, typename R>
Boolean Is(const Ref<R>& ref)
{
	return Is<T>(ref.Object());
}

template <typename T>
Ref<T> Null()
{
	return Ref<T>();
}

// New

template <typename T>
Ref<T> New()
{
	return Ref<T>(new T());
}

#ifdef GCC
template <typename T, typename A>
Ref<T> New(const InitializerList<A>& list)
{
	return Ref<T>(new T(list));
}
#endif

template <typename T, typename A>
Ref<T> New(A&& a)
{
	return Ref<T>(new T(std::forward<A>(a)));
}

template <typename T, typename A, typename B>
Ref<T> New(A&& a, B&& b)
{
	return Ref<T>(new T(std::forward<A>(a), std::forward<B>(b)));
}

template <typename T, typename A, typename B, typename C>
Ref<T> New(A&& a, B&& b, C&& c)
{
	return Ref<T>(new T(std::forward<A>(a), std::forward<B>(b), 
		std::forward<C>(c)));
}

// Ref<T>

template <typename T>
void Ref<T>::Lock()
{
	if (instance != null)
	{
		auto managed = As<IManagedType>(instance);
		if (managed == null)
			ThrowNullReferenceException(ThisMethod, "managed");
		managed->_Lock();
	}
}

template <typename T>
void Ref<T>::Unlock()
{
	if (instance != null)
	{
		auto managed = As<IManagedType>(instance);
		if (managed == null)
			ThrowNullReferenceException(ThisMethod, "managed");
		managed->_Unlock();
	}
	instance = null;
}

template <typename T>
Ref<T>::Ref()
	: instance(null)
{
}

template <typename T>
Ref<T>::Ref(T* object)
	: instance(object)
{
	Lock();
}

template <typename T>
Ref<T>::Ref(const Ref<T>& ref)
{
	instance = ref.instance;
	Lock();
}

template <typename T>
Ref<T>::~Ref()
{
	Unlock();
}

template <typename T>
Ref<T>::operator T* () const
{
	return instance;
}

template <typename T>
T* Ref<T>::operator -> () const
{
	if (instance == null)
		ThrowNullReferenceException(ThisMethod, "instance");
	return instance;
}
	
template <typename T>
Ref<T>& Ref<T>::operator = (const Ref<T>& ref)
{
	if (instance == ref.instance)
		return *this;
	Unlock();
	instance = ref.instance;
	Lock();
	return *this;
}

template <typename T>
Ref<T>& Ref<T>::operator = (T* object)
{
	if (instance == object)
		return *this;
	Unlock();
	instance = object;
	Lock();
	return *this;
}

template <typename T>
Boolean Ref<T>::operator == (T* object) const
{
	if (instance == object)
		return true;
	else if (instance == null)
		return false;
	else if (object == null)
		return false;
	else
		return *instance == *object;
}

template <typename T>
void Ref<T>::Clear()
{
	Unlock();
}

template <typename T>
Boolean Ref<T>::HasValue() const
{
	return instance != null;
}
	
template <typename T>
T& Ref<T>::Value() const
{
	if (instance == null)
		ThrowNullReferenceException(ThisMethod, "instance");
	return *instance;
}

template <typename T>
String Ref<T>::ToFormat(const String& format) const
{
	auto v = As<ValueType>(instance);
	return v == null ? "null" : v->ToFormat(format);
}

}
