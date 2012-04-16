#pragma once
#include "Codebot/Throw.h"

namespace Codebot
{

class ValueType
{
public:
	Boolean operator == (const ValueType& instance) const;
	virtual Boolean Equals(ValueType* instance) const;
	virtual String ToFormat(const String& format) const;
	String ClassName() const;
	String ToString() const;
};

template <typename T>
Boolean ValuesEqual(T* that, ValueType* instance)
{
	if (instance == null)
		return false;
	T* t = As<T>(instance);
	return t == null ? false : *that == *t;
}

}
