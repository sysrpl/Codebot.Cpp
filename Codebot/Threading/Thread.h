#pragma once
#include <Codebot/Object.h>

namespace Codebot
{
namespace Threading
{

class Thread;

typedef void (*ThreadProc)(Thread* thread, Pointer state);

struct ThreadCallback
{
	void Continue(Thread* thread);
};

class Thread : public Object
{
private:
	Handle handle;
	ThreadProc proc;
	Pointer state;
public:
	friend class ThreadCallback;
	Thread(ThreadProc proc, Pointer state);
	static void Sleep(Cardinal time);
	Boolean Wait();
	Boolean operator == (const Thread& thread) const;
	Boolean operator != (const Thread& thread) const;
};

}
}
