#include <Codebot/Interop/IStream.h>

namespace Codebot
{
namespace Interop
{

const Guid IStream::Id = "{5498E774-B3D1-4C80-95F8-7573EF266E29}";

// StreamAdapter class

StreamAdapter::StreamAdapter(Stream* s) :
	stream(s)
{
}

// StreamAdapter::IInterface

Result stdcall StreamAdapter::QueryInterface(const GuidBytes& id, Pointer& obj)
{
	if (GetInterface<IStream>(id, IStream::Id(), obj))
		return ROk;
	return InterfacedObject::QueryInterface(id, obj);
}

Integer stdcall StreamAdapter::AddRef()
{
	return _Lock();
}

Integer stdcall StreamAdapter::Release()
{
	return _Unlock();
}

// StreamAdapter::IStream

Cardinal stdcall StreamAdapter::Read(Pointer data, Cardinal count)
{
	return stream->Read(data, count);
}

Cardinal stdcall StreamAdapter::Write(Pointer data, Cardinal count)
{
	return stream->Write(data, count);
}

Cardinal stdcall StreamAdapter::Seek(Cardinal origin, Integer cursor)
{
	return stream->Seek((StreamOrigin)origin, cursor);
}

}
}
