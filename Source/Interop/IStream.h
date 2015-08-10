#pragma once
#include <Codebot/Stream.h>
#include <Codebot/Interop/IInterface.h>
#include <Codebot/Interop/InterfacedObject.h>

namespace Codebot
{
namespace Interop
{

// IStream interface

struct IStream : public IInterface
{
	static const Guid Id;
	virtual Cardinal stdcall Read(Pointer data, Cardinal count) = 0;
	virtual Cardinal stdcall Write(Pointer data, Cardinal count) = 0;
	virtual Cardinal stdcall Seek(Cardinal origin, Integer cursor) = 0;
};

// StreamAdapter class

class StreamAdapter : public InterfacedObject, public IStream
{
private:
	Ref<Stream> stream;
public:
	StreamAdapter(Stream* s);
	// IInterface
	Result stdcall QueryInterface(const GuidBytes& id, Pointer& obj);
	Integer stdcall AddRef();
	Integer stdcall Release();
	// IStream
	Cardinal stdcall Read(Pointer data, Cardinal count);
	Cardinal stdcall Write(Pointer data, Cardinal count);
	Cardinal stdcall Seek(Cardinal origin, Integer cursor);
};

}
}
