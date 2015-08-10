#pragma once
#include <Codebot/Macros.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include <vector>

namespace Codebot
{

typedef unsigned char Byte;
typedef unsigned short Word;
typedef signed int Integer;
typedef unsigned long Cardinal;
typedef signed long LongInt;
typedef unsigned long LongWord;
typedef signed long long LargeInt;
typedef unsigned long long LargeWord;
typedef void* Pointer;
typedef bool Boolean;
typedef long LongBool;
typedef float Single;
typedef double Double;
typedef float Float;
typedef unsigned long Handle;
typedef unsigned long Result;

class String;

typedef char Char;
#define PChar char*
#define null nullptr

#define Func std::function
typedef Func<void(void)> Proc;

#define Shared std::shared_ptr
#define MakeShared std::make_shared

#define Unique std::unique_ptr
#define MakeUnique std::make_unique

#ifdef GCC
#define InitializerList std::initializer_list
#endif

// Casting routines

template <typename T>
inline T& Var(const T& value)
{
	const T* v = &value;
	return *const_cast<T*>(v);
}

template <typename T>
inline Byte* VarBytes(const T& value)
{
	const T* v = &value;
	return (Byte*)const_cast<T*>(v);
}

template <typename T, typename I>
T& Cast(const I& instance)
{
	return static_cast<T&>(instance);
}

template <typename T, typename I>
T& Cast(const Shared<I>& instance)
{
	return static_cast<T&>(*instance.get());
}

// As<T>

template <typename T, typename I>
typename std::enable_if<std::is_polymorphic<I>::value, T*>::type
As(I& instance)
{
	return dynamic_cast<T*>(&instance);
}

template <typename T, typename I>
typename std::enable_if<!(std::is_pointer<I>::value ||
	std::is_polymorphic<I>::value), T*>::type
As(const I& instance)
{
	return null;
}

template <typename T, typename I>
typename std::enable_if<std::is_pointer<I>::value, T*>::type
As(const I& instance)
{
	return instance == null ? null : As<T>(*instance);
}

template <class T, class I>
T* As(const Shared<I>& instance)
{
	return dynamic_cast<T*>(instance.get());
}

// Is

template <typename T, typename I>
typename std::enable_if<std::is_polymorphic<I>::value, Boolean>::type
Is(I& instance)
{
	return dynamic_cast<T*>(&instance) != null;
}

template <typename T, typename I>
typename std::enable_if<!(std::is_pointer<I>::value ||
	std::is_polymorphic<I>::value), Boolean>::type
Is(const I& instance)
{
	return false;
}

template <typename T, typename I>
typename std::enable_if<std::is_pointer<I>::value, Boolean>::type
Is(const I& instance)
{
	return instance == null ? false : Is<T>(*instance);
}

template <typename T, typename I>
Boolean Is(const Shared<I>& instance)
{
	return dynamic_cast<T*>(instance.get()) != null;
}

// Numeric routines

template <typename T>
T Floor(const T& a)
{
	int b = (Integer)a;
	if (a == b)
		return b;
	return b < a ? b : -(1 - b);
}

template <typename T>
T Ceil(const T& a)
{
	Integer b = (Integer)a;
	if (a == b)
		return b;
	return b < a ? b + 1: -(1 - b) + 1;
}

template <typename T>
T Fract(const T& a)
{
	return a - Floor(a);
}

template <typename T>
T Round(const T& a)
{
	return Floor(a + 0.5);
}

template <typename T>
T Min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template <typename T>
T Max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template <typename T>
T Abs(const T& a)
{
	return a < 0 ? -a : a;
}

template <typename T>
T Clamp(const T& a, const T& b, const T& c)
{
	return a < b ? b : a > c ? c : a;
}

template <typename T>
T Clamp(const T& a)
{
	return a < 0 ? 0 : a > 1 ? 1 : a;
}

template <typename T>
inline void Swap(T& a, T& b)
{
	T c = a;
	a = b;
	b = c;
}

template <typename T>
inline Boolean IsOdd(const T& a)
{
	return (a & 0x1) == 0x1;
}

template <typename T>
inline Boolean IsEven(const T& a)
{
	return (a ^ 0x1) == 0x1;
}

// Common interfaces

template <typename Type, typename Indexer>
class IArray
{
public:
	virtual ~IArray() { }
	virtual Type& Item(const Indexer& index) = 0;
	virtual const Type& Item(const Indexer& index) const = 0;
	virtual Type& operator [] (const Indexer& index) = 0;
	virtual const Type& operator [] (const Indexer& index) const = 0;
	virtual Cardinal Length() const = 0;
};

struct ISerializable
{
	virtual ~ISerializable() { }
	virtual String ToFormat(const String& format) const = 0;
	virtual Boolean FromFormat(const String& value, const String& format) = 0;
};

}

