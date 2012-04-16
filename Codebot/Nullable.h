#pragma once
#include "Codebot/Text/Format.h"

namespace Codebot
{

template <typename T>
class Nullable : public ValueType
{
private:
	T* data;
public:
	Nullable();
	Nullable(const Nullable& value);
	Nullable(const T& value);
	Nullable(const T* value);
	~Nullable();
	operator T& () const;
	T& operator ()() const;
	T* operator -> () const;
	Nullable<T>& operator ()(const T& value);
	Nullable<T>& operator ()(const T* value);
	Nullable<T>& operator = (const Nullable& value);
	Nullable<T>& operator = (const T& value);
	Nullable<T>& operator = (T* value);
	Boolean operator == (const T& value) const;
	Boolean operator != (const T& value) const;
	Boolean operator < (const T& value) const;
	Boolean operator > (const T& value) const;
	Boolean operator <= (const T& value) const;
	Boolean operator >= (const T& value) const;
	// Override methods
	String ToFormat(const String& format) const;
	// Properties
	Boolean HasValue() const;
	T& Value() const;
};

template <typename T>
Nullable<T>::Nullable() :
	data(null)
{
}

template <typename T>
Nullable<T>::Nullable(const Nullable& value)
{
	data = value.data == null ? null : new T(*value.data);
}

template <typename T>
Nullable<T>::Nullable(const T& value)
{
	data = new T(value);
}

template <typename T>
Nullable<T>::Nullable(const T* value)
{
	data = value == null ? null : new T(*value);
}

template <typename T>
Nullable<T>::~Nullable()
{
	if (data != null)
		delete data;
}

template <typename T>
Nullable<T>::operator T& () const
{
	return Value();
}

template <typename T>
T& Nullable<T>::operator ()() const
{
	return Value();
}

template <typename T>
T* Nullable<T>::operator -> () const
{
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	return data;
}

template <typename T>
Nullable<T>& Nullable<T>::operator ()(const T& value)
{
	return (*this) = value;
}

template <typename T>
Nullable<T>& Nullable<T>::operator ()(const T* value)
{
	return (*this) = value;
}

template <typename T>
Nullable<T>& Nullable<T>::operator = (const Nullable& value)
{
	if (data == value.data)
		return *this;
	auto prior = data;
	data = value.data == null ? null : new T(*value.data);
	if (prior != null)
		delete prior;
	return *this;
}

template <typename T>
Nullable<T>& Nullable<T>::operator = (const T& value)
{
	auto prior = data;
	data = new T(value);
	if (prior != null)
		delete prior;
	return *this;
}

template <typename T>
Nullable<T>& Nullable<T>::operator = (T* value)
{
	if (data == value)
		return *this;
	auto prior = data;
	data = value == null ? null : new T(*value);
	if (prior != null)
		delete prior;
	return *this;
}

template <typename T>
Boolean Nullable<T>::operator == (const T& value) const
{
	return Value() == value;
}

template <typename T>
Boolean Nullable<T>::operator != (const T& value) const
{
	return Value() != value;
}

template <typename T>
Boolean Nullable<T>::operator < (const T& value) const
{
	return Value() < value;
}

template <typename T>
Boolean Nullable<T>::operator > (const T& value) const
{
	return Value() > value;
}

template <typename T>
Boolean Nullable<T>::operator <= (const T& value) const
{
	return Value() <= value;
}

template <typename T>
Boolean Nullable<T>::operator >= (const T& value) const
{
	return Value() >= value;
}

template <typename T>
String Nullable<T>::ToFormat(const String& format) const
{
	return data == null ? "null" : Text::FormatValue(*data, format);
}

// Properties

template <typename T>
Boolean Nullable<T>::HasValue() const
{
	return data != null;
}

template <typename T>
T& Nullable<T>::Value() const
{
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	return Var(*data);
}

}
