#pragma once
#include <Codebot/Interop/Guid.h>

namespace Codebot
{
namespace Interop
{

extern const Result ROk;
extern const Result RFalse;
extern const Result RNoInterface;
extern const Result RUnexpected;
extern const Result RNotImplemented;

struct IInterface
{
	static const Guid Id;
	virtual Result stdcall QueryInterface(const GuidBytes& id, Pointer& obj) = 0;
	virtual Integer stdcall AddRef() = 0;
	virtual Integer stdcall Release() = 0;
};

}
}
