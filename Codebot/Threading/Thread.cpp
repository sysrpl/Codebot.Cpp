#include "Codebot/Threading/Thread.h"
#include "Codebot/Interop/ThreadInterop.h"

namespace Codebot
{
namespace Threading
{

// ThreadCallback

void ThreadCallback::Continue(Thread* thread)
{
	thread->proc(thread, thread->state);
}

void ThreadRun(Pointer state)
{
	ThreadCallback callback;
	callback.Continue((Thread*)state);
}

//  Thread

void Thread::Sleep(Cardinal time)
{
	Interop::ThreadSleep(time);
}

Thread::Thread(ThreadProc proc, Pointer state)
{
	this->proc = proc;
	this->state = state;
	handle = Interop::ThreadStart(ThreadRun, this);
}

Boolean Thread::operator == (const Thread& thread) const
{
	return handle == thread.handle;
}

Boolean Thread::operator != (const Thread& thread) const
{
	return handle != thread.handle;
}

Boolean Thread::Wait()
{
	if (handle != 0)
		return Interop::ThreadWait(handle);
	return false;
}

}
}
