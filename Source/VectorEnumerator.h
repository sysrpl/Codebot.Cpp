#pragma once
#include <Codebot/IManagedType.h>
#include <Codebot/IEnumerable.h>
#include <Codebot/ValueType.h>
#include <Codebot/Interop/CoreInterop.h>

namespace Codebot
{

template <typename T>
class VectorEnumerator : public ValueType, public IManagedType, public IEnumerator<T>
{
private:
	Integer count;
	Integer index;
	Integer length;
	Shared<std::vector<T>> vector;
public:
	VectorEnumerator(const std::vector<T>& v);
	// IManagedType<T>
	Integer _Lock();
	Integer _Unlock();
	// IEnumerator<T>
	T Type();
	void Reset();
	Boolean HasNext();
	T* Next();
};

template <typename T>
VectorEnumerator<T>::VectorEnumerator(const std::vector<T>& v)
	: count(0), index(0), length(v.size())
{
	vector = MakeShared<std::vector<T>>(v);
}

// VectorEnumerator<T>::IManagedType

template <typename T>
Integer VectorEnumerator<T>::_Lock()
{
	return Interop::AtomicIncrement(count);
}

template <typename T>
Integer VectorEnumerator<T>::_Unlock()
{
	Integer c = Interop::AtomicDecrement(count);
	if (c == 0)
		delete this;
	return c;
}

// VectorEnumerator<T>::IEnumerator<T>

template <typename T>
void VectorEnumerator<T>::Reset()
{
	index = 0;
}

template <typename T>
Boolean VectorEnumerator<T>::HasNext()
{
	return length > index;
}

template <typename T>
T* VectorEnumerator<T>::Next()
{
	if (index < 0)
		return null;
	if (index >= length)
		return null;
	return &(*vector)[index++];
}

}
