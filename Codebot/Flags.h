#pragma once

namespace Codebot
{

typedef unsigned long IntFlag;

template <typename T>
T Set(T a)
{
	return a;
}

template <typename T>
T Set(T a, T b)
{
	return (T)((IntFlag)a | (IntFlag)b);
}

template <typename T>
T Set(T a, T b, T c)
{
	return (T)((IntFlag)a | (IntFlag)b | (IntFlag)c);
}

template <typename T>
T Set(T a, T b, T c, T d)
{
	return (T)((IntFlag)a | (IntFlag)b | (IntFlag)c | (IntFlag)d);
}

template <typename T>
T Set(T a, T b, T c, T d, T e)
{
	return (T)((IntFlag)a | (IntFlag)b | (IntFlag)c | (IntFlag)d | (IntFlag)e);
}

// Flags class

template <typename T>
class Flags
{
private:
	T flags;
public:
	Flags();
	Flags(T a);
	operator T () const;
	bool operator != (T a) const;
	bool operator == (T a) const;
	Flags<T> operator + (T a) const;
	Flags<T> operator - (T a) const;
	Flags<T>& operator += (T a);
	Flags<T>& operator -= (T a);
	bool Contains(T a) const;
	bool Equals(T a) const;
	Flags<T>& Include(T a);
	Flags<T>& Exclude(T a);
	IntFlag Value() const;
	void Value(IntFlag value);
};

template <typename T>
Flags<T>::Flags()
	: flags()
{
}

template <typename T>
Flags<T>::Flags(T a)
	: flags(a)
{
}

template <typename T>
Flags<T>::operator T () const
{
	return flags;
}

template <typename T>
bool Flags<T>::operator != (T a) const
{
	return (IntFlag)flags != (IntFlag)a;
}

template <typename T>
bool Flags<T>::operator == (T a) const
{
	return (IntFlag)flags == (IntFlag)a;
}

template <typename T>
Flags<T> Flags<T>::operator + (T a) const
{
	return Flags<T>((IntFlag)flags | (IntFlag)a);
}

template <typename T>
Flags<T> Flags<T>::operator - (T a) const
{
	return Flags<T>((IntFlag)flags & ~(IntFlag)a);
}

template <typename T>
Flags<T>& Flags<T>::operator += (T a)
{
	flags = (T)((IntFlag)flags | (IntFlag)a);
	return *this;

}

template <typename T>
Flags<T>& Flags<T>::operator -= (T a)
{
	flags = (T)((IntFlag)flags & ~(IntFlag)a);
	return *this;
}

template <typename T>
bool Flags<T>::Contains(T a) const
{
	return ((IntFlag)flags & (IntFlag)a) == (IntFlag)a;
}

template <typename T>
bool Flags<T>::Equals(T a) const
{
	return (IntFlag)flags == (IntFlag)a;
}

template <typename T>
Flags<T>& Flags<T>::Include(T a)
{
	(IntFlag)flags |= (IntFlag)a;
	return *this;
}

template <typename T>
Flags<T>& Flags<T>::Exclude(T a)
{
	(IntFlag)flags &= ~(IntFlag)a;
	return *this;
}

template <typename T>
IntFlag Flags<T>::Value() const
{
	return (IntFlag)flags;
}

template <typename T>
void Flags<T>::Value(IntFlag value)
{
	flags = (T)value;
}

}
