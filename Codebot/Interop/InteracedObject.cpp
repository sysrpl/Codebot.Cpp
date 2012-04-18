#include <Codebot/Interop/InterfacedObject.h>

namespace Codebot
{
namespace Interop
{

Result InterfacedObject::QueryInterface(const GuidBytes& id, Pointer& obj)
{
	return GetInterface<IInterface>(id, IInterface::Id(), obj) ? ROk : RNotImplemented;
}

Integer InterfacedObject::AddRef()
{
	return _Lock();
}

Integer InterfacedObject::Release()
{
	return _Unlock();
}

}
}
