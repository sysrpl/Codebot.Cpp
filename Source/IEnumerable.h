#pragma once
#include <Codebot/Types.h>

namespace Codebot
{

// IEnumerator interface

template <typename T>
class IEnumerator
{
public:
	virtual ~IEnumerator() { }
	virtual void Reset() = 0;
	virtual Boolean HasNext() = 0;
	virtual T* Next() = 0;
};

// IEnumerable interface

template <typename T>
class IEnumerable
{
public:
	virtual ~IEnumerable() { }
	virtual IEnumerator<T>* GetEnumerator() const = 0;
};

}
