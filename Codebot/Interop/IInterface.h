#pragma once
#include <Codebot/Interop/Guid.h>

namespace Codebot
{
namespace Interop
{

struct IInterface
{
	static const Guid Id;
	virtual Result stdcall QueryInterface(const GuidBytes& id, Pointer& obj) = 0;
	virtual Integer stdcall AddRef() = 0;
	virtual Integer stdcall Release() = 0;
};

}
}
