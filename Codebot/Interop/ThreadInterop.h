#pragma once
#include "Codebot/Types.h"

namespace Codebot
{
namespace Interop
{

typedef void (*ThreadProc)(Pointer State);

extern "C"
{
	import Handle ThreadStart(ThreadProc proc, Pointer state);
	import Boolean ThreadWait(Handle& handle);
	import void ThreadSleep(Cardinal time);
};

}
}
