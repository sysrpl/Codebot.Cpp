#pragma once
#include <Codebot/Throw.h>

namespace Codebot
{

class IManagedType
{
public:
	virtual ~IManagedType() { }
	virtual void _Lock() = 0;
	virtual void _Unlock() = 0;
};

template <typename T>
T* Lock(T* instance)
{
	auto m = As<IManagedType>(instance);
	if (m == null)
		ThrowCastException(ThisMethod, "IManagedType");
	m->_Lock();
	return instance;
}

template <typename T>
void Unlock(T*& instance)
{
	auto m = As<IManagedType>(instance);
	if (m == null)
		ThrowCastException(ThisMethod, "IManagedType");
	m->_Unlock();
	instance = null;
}

}
