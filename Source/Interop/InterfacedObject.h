#pragma once
#include <Codebot/Object.h>
#include <Codebot/Interop/IInterface.h>
#include <Codebot/Memory/MemoryTools.h>

namespace Codebot
{
namespace Interop
{

class InterfacedObject : public Object, public IInterface
{
protected:
	template <typename T>
	Boolean GetInterface(const GuidBytes& a, const GuidBytes& b, Pointer& obj);
public:
	Result stdcall QueryInterface(const GuidBytes& id, Pointer& obj);
	Integer stdcall AddRef();
	Integer stdcall Release();
};

template <typename T>
Boolean InterfacedObject::GetInterface(const GuidBytes& a, const GuidBytes& b, Pointer& obj)
{
	obj = null;
	if (Memory::MemCompare(a, b))
		obj = (Pointer)As<T>(this);
	return obj != null;
}

}
}
